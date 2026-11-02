#!/usr/bin/env python3
"""SystemView IP Recorder handshake followed by bidirectional OpenOCD RTT.

IP protocol follows SEGGER Sample/COMM/IP/SEGGER_SYSVIEW_COMM_IP_embOS_emNet.c:
32-byte hello, length-prefixed host commands, and raw target event data.
Only one recorder can consume the RTT channel at a time.
"""
import argparse
import asyncio
import logging
import os
import tempfile
from collections import deque

async def disk_io(func, *args):
    task = asyncio.create_task(asyncio.to_thread(func, *args))
    try:
        return await asyncio.shield(task)
    except asyncio.CancelledError:
        # The worker thread cannot be cancelled; finish it before closing its fd.
        await task
        raise

class CaptureBuffer:
    """FIFO with a RAM budget and a session-local disk spill file.

    Producer never waits for the network consumer. Disk I/O runs off the event
    loop, with positional reads/writes so collection and transmission can overlap.
    """
    def __init__(self, memory_bytes, spool_dir=None):
        self.memory_bytes = memory_bytes
        self.memory_used = 0
        self.pending_bytes = 0
        self.peak_bytes = 0
        self.spilled_bytes = 0
        self.items = deque()
        self.ready = asyncio.Event()
        self.finished = False
        self.file = None
        self.spool_dir = spool_dir
        self.offset = 0

    async def put(self, data):
        if self.memory_used + len(data) <= self.memory_bytes:
            self.memory_used += len(data)
            self.items.append(data)
        else:
            if self.file is None:
                self.file = tempfile.TemporaryFile(dir=self.spool_dir)
                logging.warning("Capture RAM budget reached; spilling trace to local disk")
            offset = self.offset
            await disk_io(self._write, offset, data)
            self.offset += len(data)
            self.spilled_bytes += len(data)
            self.items.append((offset, len(data)))
        self.pending_bytes += len(data)
        self.peak_bytes = max(self.peak_bytes, self.pending_bytes)
        self.ready.set()

    def _write(self, offset, data):
        view = memoryview(data)
        while view:
            count = os.pwrite(self.file.fileno(), view, offset)
            if count <= 0:
                raise OSError("Failed to spool captured trace")
            offset += count
            view = view[count:]

    async def get(self):
        while not self.items:
            if self.finished:
                return None
            self.ready.clear()
            await self.ready.wait()
        item = self.items.popleft()
        if isinstance(item, bytes):
            data = item
            self.memory_used -= len(data)
        else:
            offset, size = item
            data = await disk_io(os.pread, self.file.fileno(), size, offset)
            if len(data) != size:
                raise OSError("Incomplete captured trace in spool file")
        return data

    def finish(self):
        self.finished = True
        self.ready.set()

    def close(self):
        if self.file is not None:
            self.file.close()

async def collect(reader, buffer):
    try:
        while data := await reader.read(65536):
            await buffer.put(data)
    finally:
        buffer.finish()

async def transmit(buffer, writer):
    while (data := await buffer.get()) is not None:
        writer.write(data)
        await writer.drain()
        buffer.pending_bytes -= len(data)

async def relay_trace(reader, writer, buffer):
    tasks = [asyncio.create_task(collect(reader, buffer)),
             asyncio.create_task(transmit(buffer, writer))]
    try:
        # EOF from RTT finishes collection, then sends all queued bytes before exit.
        done, pending = await asyncio.wait(tasks, return_when=asyncio.FIRST_EXCEPTION)
        for task in done:
            task.result()
    finally:
        # Wait for positional file I/O before closing the session's spill file.
        for task in tasks:
            task.cancel()
        await asyncio.gather(*tasks, return_exceptions=True)

HELLO_PREFIX = b"SEGGER SystemView"
TARGET_HELLO = b"SEGGER SystemView V3.32.00".ljust(32, b"\x00")

async def relay_commands(reader, writer):
    while True:
        size = (await reader.readexactly(1))[0]
        command = await reader.readexactly(size)
        writer.write(command)
        await writer.drain()

async def serve_client(reader, writer, args):
    if args.busy:
        writer.close()
        await writer.wait_closed()
        return
    args.busy = True
    upstream = None
    buffer = CaptureBuffer(getattr(args, "buffer_mib", 256) * 1024 * 1024,
                           getattr(args, "spool_dir", None))
    try:
        hello = await asyncio.wait_for(reader.readexactly(32), 10)
        if not hello.startswith(HELLO_PREFIX):
            raise ValueError(f"Expected 32-byte SystemView IP hello, received {hello!r}")
        raw_reader, upstream = await asyncio.open_connection('127.0.0.1', args.rtt_port)
        writer.write(TARGET_HELLO)
        await writer.drain()
        logging.info('Recorder connected: %s', writer.get_extra_info('peername'))
        tasks = [asyncio.create_task(relay_commands(reader, upstream)),
                 asyncio.create_task(relay_trace(raw_reader, writer, buffer))]
        try:
            done, pending = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
            for task in done:
                task.result()
        finally:
            for task in tasks:
                task.cancel()
            await asyncio.gather(*tasks, return_exceptions=True)
    except (OSError, ValueError, asyncio.IncompleteReadError, asyncio.TimeoutError) as exc:
        logging.warning('Recorder disconnected: %s', exc)
    finally:
        for stream in (upstream, writer):
            if stream is not None:
                stream.close()
                try:
                    await asyncio.wait_for(stream.wait_closed(), 2)
                except (OSError, asyncio.TimeoutError):
                    stream.transport.abort()
        logging.info("Capture ended: peak queue=%d bytes, disk spill=%d bytes, unsent queue=%d bytes",
                     buffer.peak_bytes, buffer.spilled_bytes, buffer.pending_bytes)
        buffer.close()
        args.busy = False

async def main(args):
    args.busy = False
    server = await asyncio.start_server(lambda r, w: serve_client(r, w, args), args.bind, args.port)
    logging.info('SystemView IP Recorder listening on %s:%d', args.bind, args.port)
    async with server:
        await server.serve_forever()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bind', default='0.0.0.0')
    parser.add_argument('--port', type=int, default=19111)
    parser.add_argument('--rtt-port', type=int, default=19021)
    parser.add_argument('--buffer-mib', type=int, default=256,
                        help='RAM queue budget in MiB (default: 256); overflow spills to disk')
    parser.add_argument('--spool-dir', default=None,
                        help='Temporary local spill directory (default: system temp)')
    logging.basicConfig(level=logging.INFO, format='%(message)s')
    try:
        args = parser.parse_args()
        if args.buffer_mib < 1:
            parser.error('--buffer-mib must be at least 1')
        asyncio.run(main(args))
    except KeyboardInterrupt:
        pass
