import websockets
import asyncio

async def connect_and_send(messages):
    uri = "ws://192.168.4.1/ws"
    async with websockets.connect(uri) as websocket:
        for message in messages:
            await websocket.send(message)

forward = '{"controller":"engines","command":"FORWARD"}'
backward_left = '{"controller":"engines","command":"BACKWARD_LEFT"}'
set_speed = '{"controller":"engines","command":"SPEED","speed":1000}'

commands = [forward, backward_left, set_speed]

asyncio.get_event_loop().run_until_complete(connect_and_send(commands))
