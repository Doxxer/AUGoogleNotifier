import BaseHTTPServer
from SocketServer import TCPServer
import webbrowser


class GetAndStopHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    # noinspection PyPep8Naming
    def do_GET(self):
        request = self.path
        assert isinstance(request, str)
        if request.startswith("/?code="):
            self.server.authenticate_code = request.split("/?code=")[1]
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write("<html>")
            self.wfile.write("<body onload=\"window.open('','_self','').close();\"/>")
            self.wfile.write("</html>")
            self.wfile.close()
            self.server.stop = True


class StoppableHttpServer(BaseHTTPServer.HTTPServer):
    def __init__(self, server_address, RequestHandlerClass, bind_and_activate=True):
        TCPServer.__init__(self, server_address, RequestHandlerClass, bind_and_activate=True)
        self.authenticate_code = None
        self.stop = False

    def serve_forever(self, poll_interval=0.5):
        while not self.stop:
            self.handle_request()


def grab_authorization_code(url, port_number=19004):
    server = StoppableHttpServer(("", port_number), RequestHandlerClass=GetAndStopHandler)
    webbrowser.open_new(url)
    server.serve_forever()
    return server.authenticate_code