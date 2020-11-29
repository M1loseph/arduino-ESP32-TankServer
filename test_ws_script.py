import websockets
import asyncio
import json
from time import sleep


async def connect_and_send(messages):
    uri = "ws://192.168.4.1/ws"
    async with websockets.connect(uri) as websocket:
        for message in messages:
            await websocket.send(message)
            sleep(0.5)

engines = [
    {
        "controller": "engines",
        "command": "forward",
        "engines": "both"
    },
    {
        "controller": "engines",
        "command": "forward",
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "forward",
        "engines": "right"
    },
    {
        "controller": "engines",
        "command": "backward",
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "stop",
        "engines": "both"
    },
    {
        "controller": "engines",
        "command": "stop",
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "stop",
        "engines": "right"
    },
    {
        "controller": "engines",
        "command": "slower",
        "engines": "both"
    },
    {
        "controller": "engines",
        "command": "slower",
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "slower",
        "engines": "right"
    },
    {
        "controller": "engines",
        "command": "keep_speed",
        "engines": "both"
    },
    {
        "controller": "engines",
        "command": "keep_speed",
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "keep_speed",
        "engines": "right"
    },
    {
        "controller": "engines",
        "command": "speed",
        "speed": 1023,
        "engines": "both"
    },
    {
        "controller": "engines",
        "command": "speed",
        "speed": 1023,
        "engines": "left"
    },
    {
        "controller": "engines",
        "command": "speed",
        "speed": 1023,
        "engines": "right"
    },
]

engines = [json.dumps(e) for e in engines]

asyncio.get_event_loop().run_until_complete(connect_and_send(engines))
