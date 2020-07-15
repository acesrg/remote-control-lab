import asyncio
import time
import unittest

import json
import websockets

AEROPENDULUM_IP_ADD = "192.168.1.41"


class WebsocketsTestCase(unittest.TestCase):

    def setUp(self):
        self.loop = asyncio.get_event_loop()
        self.json_data = {"duty": "0x0000"}
        self.message = json.dumps(self.json_data)

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

    async def a_test_classic_control_response(self):
        uri = "classic"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        #send to activate callback and receive
        await websocket.send(self.message)
        response = await websocket.recv()
        await websocket.close()
        parsed_response = json.loads(response)

        self.assertIsInstance(int(parsed_response['angle'], 16), int)
        self.assertIsInstance(int(parsed_response['error'], 16), int)

    def test_classic_control_response(self):
        self.loop.run_until_complete(self.a_test_classic_control_response())


    async def a_test_json_parsing(self):
        uri = "test"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        await websocket.send(self.message)
        response = await websocket.recv()
        await websocket.close()

        parsed_response = json.loads(response)

        self.assertEqual(parsed_response, self.json_data)

    def test_json_parsing(self):
        self.loop.run_until_complete(self.a_test_json_parsing())


    async def a_test_pwm_start(self):
        uri = "start_pwm"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)

        time.sleep(2)

        await websocket.close()

    def test_pwm_start(self):
        self.loop.run_until_complete(self.a_test_pwm_start())
