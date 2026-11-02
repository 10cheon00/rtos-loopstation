import asyncio, importlib.util, types
from pathlib import Path
spec=importlib.util.spec_from_file_location('bridge',str(Path(__file__).with_name('bridge.py')))
b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b)
async def run():
    received=asyncio.Queue()
    async def raw(r,w):
        data=await r.readexactly(3);await received.put(data)
        w.write(b'trace');await w.drain();w.close();await w.wait_closed()
    raw_server=await asyncio.start_server(raw,'127.0.0.1',0)
    args=types.SimpleNamespace(busy=False,rtt_port=raw_server.sockets[0].getsockname()[1])
    server=await asyncio.start_server(lambda r,w:b.serve_client(r,w,args),'127.0.0.1',0)
    for _ in range(2):
        r,w=await asyncio.open_connection('127.0.0.1',server.sockets[0].getsockname()[1])
        w.write(b'SEG');await w.drain();await asyncio.sleep(.01)
        w.write(b'GER SystemView'.ljust(29,b'\x00')+b'\x02\x01');await w.drain()
        await asyncio.sleep(.01)
        w.write(b'\x02\x01\x03');await w.drain()
        assert await r.readexactly(32)==b'SEGGER SystemView V3.32.00'.ljust(32,b'\x00')
        assert await r.readexactly(5)==b'trace'
        assert await received.get()==b'\x01\x02\x03'
        assert await r.read()==b''
        w.close();await w.wait_closed();await asyncio.sleep(.05)
    server.close();raw_server.close()
    await server.wait_closed();await raw_server.wait_closed()
    print('PASS: 32-byte fragmented IP hello, fragmented/coalesced command frames, trace forwarding, reconnect')
async def slow_network(memory_bytes):
    chunks = [bytes([i % 251]) * 4096 for i in range(128)]
    class Source:
        def __init__(self):
            self.index = 0
        async def read(self, size):
            if self.index == len(chunks):
                return b''
            data = chunks[self.index]
            self.index += 1
            return data
    class SlowWriter:
        def __init__(self):
            self.release = asyncio.Event()
            self.blocked = asyncio.Event()
            self.output = bytearray()
        def write(self, data):
            self.output.extend(data)
        async def drain(self):
            self.blocked.set()
            await self.release.wait()
    source, writer = Source(), SlowWriter()
    buffer = b.CaptureBuffer(memory_bytes)
    transfer = asyncio.create_task(b.relay_trace(source, writer, buffer))
    try:
        await asyncio.wait_for(writer.blocked.wait(), 2)
        # Windows is stalled throughout this wait. RTT must still reach EOF.
        async with asyncio.timeout(5):
            while not buffer.finished:
                await asyncio.sleep(.001)
        assert source.index == len(chunks)
        assert not transfer.done(), 'Sender must wait for Windows without stopping capture'
        assert buffer.memory_used <= memory_bytes
        if memory_bytes < 4096 * len(chunks):
            assert buffer.spilled_bytes > 0
        else:
            assert buffer.spilled_bytes == 0
        writer.release.set()
        await asyncio.wait_for(transfer, 5)
        assert writer.output == b''.join(chunks), 'FIFO bytes must survive disk/RAM mixing'
        assert buffer.pending_bytes == 0
    finally:
        transfer.cancel()
        await asyncio.gather(transfer, return_exceptions=True)
        buffer.close()

async def cancelled_transfer():
    source = asyncio.StreamReader()
    source.feed_data(b'x' * 131072)
    class BlockedWriter:
        def write(self, data): pass
        async def drain(self): await asyncio.Future()
    buffer = b.CaptureBuffer(1)
    transfer = asyncio.create_task(b.relay_trace(source, BlockedWriter(), buffer))
    await asyncio.sleep(.02)
    transfer.cancel()
    await asyncio.gather(transfer, return_exceptions=True)
    buffer.close()

async def disk_failure():
    source = asyncio.StreamReader()
    source.feed_data(b'x' * 1024)
    source.feed_eof()
    class Writer:
        def write(self, data): pass
        async def drain(self): pass
    buffer = b.CaptureBuffer(1)
    def fail_write(offset, data):
        raise OSError('simulated disk full')
    buffer._write = fail_write
    try:
        try:
            await asyncio.wait_for(b.relay_trace(source, Writer(), buffer), 2)
        except OSError as exc:
            assert 'simulated disk full' in str(exc)
        else:
            raise AssertionError('Disk failure must be reported, not silently drop data')
    finally:
        buffer.close()

async def main():
    await run()
    await slow_network(1024 * 1024)  # Entire backlog fits in memory.
    await slow_network(8192)         # Tiny budget forces local disk spill.
    await cancelled_transfer()
    await disk_failure()
    print('PASS: stalled sender does not stop RTT capture; RAM budget, disk FIFO, EOF drain, cancellation')

asyncio.run(main())
