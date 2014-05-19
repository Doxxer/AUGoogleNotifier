from validateCredential import validateCredential
from handlers.BaseHandler import BaseHandler


class LogoutHandler(BaseHandler):
    @validateCredential
    def post(self):
        self.logout()
        self.response.set_status(200)
        self.response.write("Logout successful")