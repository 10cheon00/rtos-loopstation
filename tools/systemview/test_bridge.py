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
asyncio.run(run())
