import BaseHTTPServer
import SimpleHTTPServer
from SocketServer import TCPServer
import webbrowser


class get_and_stop_handler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def do_GET(self):
        request = self.path
        assert isinstance(request, str)
        if request.startswith("/?code="):
            self.server.authenticate_code = request.split("/?code=")[1]
            self.server.stop = True


class StoppableHttpServer(BaseHTTPServer.HTTPServer):
    def __init__(self, server_address, RequestHandlerClass, bind_and_activate=True):
        TCPServer.__init__(self, server_address, RequestHandlerClass, bind_and_activate=True)
        self.authenticate_code = None
        self.stop = False

    def serve_forever(self):
        while not self.stop:
            self.handle_request()


def grab_authorization_code(url, port_number=19004):
    server = StoppableHttpServer(("", port_number), RequestHandlerClass=get_and_stop_handler)
    webbrowser.open_new(url)
    server.serve_forever()
    return server.authenticate_code