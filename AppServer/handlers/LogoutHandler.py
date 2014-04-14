from ValidateCredential import ValidateCredential
from handlers.BaseHandler import BaseHandler


class LogoutHandler(BaseHandler):
    def get(self):
        self.logout()
        self.response.set_status(200)