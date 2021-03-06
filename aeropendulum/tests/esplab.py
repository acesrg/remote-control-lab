from enum import Enum
from http.client import HTTPConnection as _HTTPConnection
from urllib.parse import urlparse, parse_qsl, urlencode, urlunparse


class HTTPConnection:

    def __init__(self, ip, port, timeout):
        self.con = _HTTPConnection(ip, port=port, timeout=timeout)

    def request(self, method, url):
        HARDCODED_SUPPORTED_METHOD = "GET"

        # decode
        parsed_url = urlparse(url)

        if (not parsed_url.path) or (parsed_url.path=="/"):
            updated_url = url
        else:
            query_elements = parse_qsl(parsed_url.query)

            # add method
            query_elements.append(('verb', method))

            # re encode
            updated_query = urlencode(query_elements)
            updated_parsed_url= parsed_url._replace(query=updated_query)
            updated_url = urlunparse(updated_parsed_url)

        self.con.request(HARDCODED_SUPPORTED_METHOD, updated_url)

    def getresponse(self):
        return self.con.getresponse()


class Resourcer(HTTPConnection):

    def __init__(self, ip, port, timeout):
        super().__init__(ip, port, timeout)

    @staticmethod
    def retval(mode, response):
        if mode == "payload":
            return response.read().decode().splitlines()[0]
        elif mode == "code":
            return response.code
        else:
            return response


    def get(self, resource, retval_mode="payload"):
        METHOD = "GET"
        self.request(METHOD, resource)
        return self.retval(retval_mode, self.getresponse())


    def post(self, resource, value, retval_mode="code"):
        METHOD = "POST"

        parsed_resource = urlparse(resource)

        q = urlencode([("value", value)])
        resource_with_query = parsed_resource._replace(query=q)
        unparsed_resource_with_query = urlunparse(resource_with_query)
        self.request(METHOD, unparsed_resource_with_query)

        return self.retval(retval_mode, self.getresponse())
