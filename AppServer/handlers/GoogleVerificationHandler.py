from handlers.BaseHandler import BaseHandler


class GoogleVerificationHandler(BaseHandler):
    def get(self):
        self.render('googled2ac93807b24db11.html')