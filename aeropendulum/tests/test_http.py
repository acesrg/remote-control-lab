from esplab import Resourcer
from socket import timeout
import unittest

# TODO: this should be more automatic
AEROPENDULUM_IP_ADD = "192.168.100.41"


class HTTPTestCase(unittest.TestCase):
    def setUp(self):
        retries = 3
        while retries >= 0:
            try:
                self.resourcer = Resourcer(AEROPENDULUM_IP_ADD, port=80, timeout=20)
                break
            except timeout:
                print("Timed out, retrying...")
                retries -= 1

    def test_dummy_resource(self):
        RESOURCE = "/test/resource"
        VALUE = "hithere"
        response = self.resourcer.post(RESOURCE, VALUE)
        self.assertEqual(response, 202)

        response = self.resourcer.get(RESOURCE)
        self.assertEqual(response, VALUE)

    def test_logger(self):
        RESOURCE = "/logger/level"
        LOG_LEVEL = "LOG_TRACE"
        response = self.resourcer.post(RESOURCE, LOG_LEVEL)
        self.assertEqual(response, 202)

        response = self.resourcer.get(RESOURCE)
        self.assertEqual(response, LOG_LEVEL)

        LOG_LEVEL = "LOG_INFO"
        response = self.resourcer.post(RESOURCE, LOG_LEVEL)
        self.assertEqual(response, 202)

        response = self.resourcer.get(RESOURCE)
        self.assertEqual(response, LOG_LEVEL)

    def test_telemetry_period(self):
        RESOURCE = "/telemetry/period"
        PERIOD = 20
        response = self.resourcer.post(RESOURCE, PERIOD)
        self.assertEqual(response, 202)

        response = self.resourcer.get(RESOURCE)
        self.assertEqual(int(response), PERIOD)

        DEFAULT_PERIOD = 500
        response = self.resourcer.post(RESOURCE, DEFAULT_PERIOD)
        self.assertEqual(response, 202)
