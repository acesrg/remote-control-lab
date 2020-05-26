import asyncio
import time
import unittest

import websockets

AEROPENDULUM_IP_ADD = "192.168.1.41"


class TestStringMethods(unittest.TestCase):

    async def send_to_aero(self, message):
        uri = "ws://" + AEROPENDULUM_IP_ADD
        async with websockets.connect(uri) as websocket:

            await websocket.send(message)
            response = await websocket.recv()

        return response

    def test_ping(self):
        message = "ping"
        loop = asyncio.get_event_loop()
        response = loop.run_until_complete(self.send_to_aero(message))

        self.assertEqual(response, "pong")
