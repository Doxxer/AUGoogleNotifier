import logging

from ValidateCredential import ValidateCredential
from handlers.BaseHandler import BaseHandler


class UnsubscribeHandler(BaseHandler):
    @ValidateCredential
    def post(self):
        """
        POST request handling method.

        Unsubscribe from push notifications.
        """
        result = self.Unsubscribe()
        logging.warning(result)

        if not result['success']:
            self.response.set_status(result['error_code'])
            self.response.write(result['error_msg'])
            if result['error_code'] == 401:
                self.logout()
        else:
            self.response.write('OK')
            self.response.set_status(200)