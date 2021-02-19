from esplab import HTTPConnection
import unittest

# TODO: this should be more automatic
AEROPENDULUM_IP_ADD = "192.168.100.41"


class HTTPTestCase(unittest.TestCase):
    def setUp(self):
        self.con = HTTPConnection(AEROPENDULUM_IP_ADD, port=80, timeout=5)

    def test_dummy_resource(self):
        VALUE = "hi_there"
        self.con.request("POST", "/test/resource?value=" + VALUE)
        self.assertEqual(self.con.getresponse().code, 202)

        self.con.request("GET", "/test/resource")
        res = self.con.getresponse()
        self.assertEqual(res.code, 200)
        self.assertEqual(res.read().decode('utf-8').splitlines()[0], VALUE)
