from esplab import HTTPConnection
from socket import timeout
import unittest

# TODO: this should be more automatic
AEROPENDULUM_IP_ADD = "192.168.100.41"


class HTTPTestCase(unittest.TestCase):
    def setUp(self):
        retries = 3
        while retries >= 0:
            try:
                self.con = HTTPConnection(AEROPENDULUM_IP_ADD, port=80, timeout=20)
                break
            except timeout:
                print("Timed out, retrying...")
                retries -= 1

    def test_dummy_resource(self):
        VALUE = "hi_there"
        self.con.request("POST", "/test/resource?value=" + VALUE)
        self.assertEqual(self.con.getresponse().code, 202)

        self.con.request("GET", "/test/resource")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], VALUE)

    def test_logger(self):
        LOG_LEVEL = "LOG_TRACE"
        self.con.request("POST", "/logger/level?value=" + LOG_LEVEL)
        self.assertEqual(self.con.getresponse().code, 202)

        self.con.request("GET", "/logger/level")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], LOG_LEVEL)

        LOG_LEVEL = "LOG_INFO"
        self.con.request("POST", "/logger/level?value=" + LOG_LEVEL)
        self.assertEqual(self.con.getresponse().code, 202)

        self.con.request("GET", "/logger/level")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], LOG_LEVEL)

    def test_telemetry_period(self):
        DEFAULT_PERIOD = 500
        self.con.request("GET", "/telemetry/period")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], str(DEFAULT_PERIOD))

        PERIOD = 20
        self.con.request("POST", "/telemetry/period?value={}".format(PERIOD))
        self.assertEqual(self.con.getresponse().code, 202)

        DEFAULT_PERIOD = 500
        self.con.request("GET", "/telemetry/period")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], str(format(PERIOD)))

        self.con.request("POST", "/telemetry/period?value={}".format(DEFAULT_PERIOD))
        self.assertEqual(self.con.getresponse().code, 202)
