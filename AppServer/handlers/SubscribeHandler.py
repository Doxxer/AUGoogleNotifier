from ValidateCredential import validateCredential
from handlers.BaseHandler import BaseHandler


class SubscribeHandler(BaseHandler):
    @validateCredential()
    def post(self):
        """
        POST request handling method.

        Make POST request to subscribe for Drive Files/Changes resources.
        """
        if self.session.get('notification_id'):
            result = self.unsubscribe(True)
            if not result['success']:
                self.response.set_status(result['error_code'])
                self.response.write("Error occurred while unsubscribing from channel: " + result['error_msg'])
                return

        result = self.subscribe()
        if not result['success']:
            self.response.set_status(result['error_code'])
            self.response.write("Error occurred while subscribing: " + result['error_msg'])
        else:
            self.response.set_status(200)
            self.response.write("Subscribed successfully")