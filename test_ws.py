import websockets
import asyncio

async def connect_and_send(message):
    uri = "ws://192.168.4.1/ws"
    async with websockets.connect(uri) as websocket:
        await websocket.send(message)

forward = '{"controller":"engines","command":"FORWARD"}'
backward_left = '{"controller":"engines","command":"BACKWARD_LEFT"}'
rotate_left = '{"controller":"engines","command":"ROTATE_LEFT"}'
rotate_right = '{"controller":"engines","command":"ROTATE_RIGHT"}'
set_speed = '{"controller":"engines","command":"SPEED","speed":1000}'

asyncio.get_event_loop().run_until_complete(connect_and_send(forward))
asyncio.get_event_loop().run_until_complete(connect_and_send(backward_left))
asyncio.get_event_loop().run_until_complete(connect_and_send(rotate_left))
asyncio.get_event_loop().run_until_complete(connect_and_send(rotate_right))
asyncio.get_event_loop().run_until_complete(connect_and_send(set_speed))
