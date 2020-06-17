import asyncio
import time
import unittest

import json
import websockets

AEROPENDULUM_IP_ADD = "192.168.1.41"


class WebsocketsTestCase(unittest.TestCase):

    def setUp(self):
        self.loop = asyncio.get_event_loop()

    async def connect(self, address, uri):
        url = "ws://" + address + "/" + uri
        websocket = await websockets.connect(url)
        return websocket

    async def a_test_ping(self):
        uri = "ping"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        response = await websocket.recv()
        await websocket.close()
        self.assertEqual(response, "pong")

    def test_ping(self):
        self.loop.run_until_complete(self.a_test_ping())

    async def a_test_uri(self):
        uri = "classic"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        response = await websocket.recv()
        await websocket.close()
        self.assertEqual(response, "classic control mode")

    def test_uri(self):
        self.loop.run_until_complete(self.a_test_uri())

    async def a_test_classic_control_response(self):
        uri = "classic"
        message = "some"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)

        # await for greeting
        await websocket.recv()

        #send to activate callback and receive
        await websocket.send(message)
        response = await websocket.recv()
        await websocket.close()
        parsed_response = json.loads(response)

        self.assertIsInstance(parsed_response['angle'], float)
        self.assertIsInstance(parsed_response['error'], int)

    def test_classic_control_response(self):
        self.loop.run_until_complete(self.a_test_classic_control_response())


    async def a_test_json_parsing(self):
        uri = "test"
        json_data = {"duty": 3.6}
        message = json.dumps(json_data)
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        await websocket.send(message)
        response = await websocket.recv()
        parsed_response = json.loads(response)

        self.assertEqual(parsed_response, json_data)

    def test_json_parsing(self):
        self.loop.run_until_complete(self.a_test_json_parsing())

