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

    def _test_resources(self, resource, value):
        RESOURCE = resource
        VALUE = value
        value_type = type(value)
        response = self.resourcer.post(RESOURCE, VALUE)
        self.assertEqual(response, 202)

        response = self.resourcer.get(RESOURCE)
        self.assertEqual(value_type(response), VALUE)


    def test_dummy_resource(self):
        RESOURCE = "/test/resource"
        VALUE = "hithere"
        self._test_resources(RESOURCE, VALUE)

    def test_logger(self):
        RESOURCE = "/logger/level"
        LOG_LEVEL = "LOG_TRACE"
        self._test_resources(RESOURCE, LOG_LEVEL)

        LOG_LEVEL = "LOG_INFO"
        self._test_resources(RESOURCE, LOG_LEVEL)

    def test_telemetry_period(self):
        RESOURCE = "/telemetry/period"
        PERIOD = 20
        self._test_resources(RESOURCE, PERIOD)

        DEFAULT_PERIOD = 500
        self._test_resources(RESOURCE, DEFAULT_PERIOD)
