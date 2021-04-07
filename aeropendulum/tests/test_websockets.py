import asyncio
import time
import unittest

import json
import websockets

# TODO: this should be more automatic
AEROPENDULUM_IP_ADD = "192.168.100.41"


class WebsocketsTestCase(unittest.TestCase):

    def setUp(self):
        self.loop = asyncio.get_event_loop()
        self.json_data = {"duty": "0x0000"}
        self.message = json.dumps(self.json_data)

    async def connect(self, address, uri):
        url = "ws://" + address + "/" + uri
        websocket = await websockets.connect(url)
        return websocket

    async def a_test_telemetry_received(self):
        uri = "stream"
        websocket = await self.connect(AEROPENDULUM_IP_ADD, uri)
        response = await websocket.recv()
        await websocket.close()
        parsed_response = json.loads(response)

        self.assertIsInstance(int(parsed_response['angle'], 16), int)
        self.assertIsInstance(int(parsed_response['error'], 16), int)

    def test_telemetry_received(self):
        self.loop.run_until_complete(self.a_test_telemetry_received())

    def test_hammer_server_with_connections(self):
        for i in range(10):
            self.loop.run_until_complete(self.a_test_telemetry_received())
