#!/usr/bin/env python3
"""SystemView IP Recorder handshake followed by bidirectional OpenOCD RTT.

IP protocol follows SEGGER Sample/COMM/IP/SEGGER_SYSVIEW_COMM_IP_embOS_emNet.c:
32-byte hello, length-prefixed host commands, and raw target event data.
Only one recorder can consume the RTT channel at a time.
"""
import argparse
import asyncio
import logging

async def relay(reader, writer):
    while data := await reader.read(65536):
        writer.write(data)
        await writer.drain()

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
    try:
        hello = await asyncio.wait_for(reader.readexactly(32), 10)
        if not hello.startswith(HELLO_PREFIX):
            raise ValueError(f"Expected 32-byte SystemView IP hello, received {hello!r}")
        raw_reader, upstream = await asyncio.open_connection('127.0.0.1', args.rtt_port)
        writer.write(TARGET_HELLO)
        await writer.drain()
        logging.info('Recorder connected: %s', writer.get_extra_info('peername'))
        tasks = [asyncio.create_task(relay_commands(reader, upstream)),
                 asyncio.create_task(relay(raw_reader, writer))]
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
                    await stream.wait_closed()
                except OSError:
                    pass
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
    logging.basicConfig(level=logging.INFO, format='%(message)s')
    try:
        asyncio.run(main(parser.parse_args()))
    except KeyboardInterrupt:
        pass
