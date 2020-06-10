import asyncio
import time
import unittest

import json
import websockets

AEROPENDULUM_IP_ADD = "192.168.1.41"


class WebsocketsTestCase(unittest.TestCase):

    async def connect_to_aero_uri(self, uri):
        url = "ws://" + AEROPENDULUM_IP_ADD + "/" + uri
        async with websockets.connect(url) as websocket:
            response = await websocket.recv()
        return response

    async def send_message_to_aero_uri(self, message, uri):
        url = "ws://" + AEROPENDULUM_IP_ADD + "/" + uri
        async with websockets.connect(url) as websocket:
            await websocket.recv()
            await websocket.send(message)
            response = await websocket.recv()

        return response

    def test_ping(self):
        uri = "ping"
        loop = asyncio.get_event_loop()
        response = loop.run_until_complete(self.connect_to_aero_uri(uri))

        self.assertEqual(response, "pong")

    def test_uri(self):
        uri = "classic"
        loop = asyncio.get_event_loop()
        response = loop.run_until_complete(self.connect_to_aero_uri(uri))

        self.assertEqual(response, "classic control mode")

    def test_classic_control_response(self):
        uri = "classic"
        message = "some"
        loop = asyncio.get_event_loop()
        response = loop.run_until_complete(self.send_message_to_aero_uri(message, uri))

        parsed_response = json.loads(response)

        self.assertIsInstance(parsed_response['angle'], float)
        self.assertIsInstance(parsed_response['error'], int)

