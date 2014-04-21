import logging
import pickle

from oauth2client.client import AccessTokenRefreshError
import httplib2
from handlers.BaseHandler import BaseHandler


class LoginHandler(BaseHandler):
    """
    Request handling class for /login
    """

    def post(self):
        """
        POST request handling method.
        TODO ???
        """
        logging.error(self.session)

        if not self.user_logged_in():
            self.response.write(self.compose_auth_url())
            return

        if self.authenticate_user():
            self.response.set_status(200)
            self.response.write("OK " + self.session['name'])

    def user_logged_in(self):
        return 'credential' in self.session and \
               'mac_address' in self.session and \
               self.session['mac_address'] == self.get_value_from_cookie('mac_address')

    def compose_auth_url(self):
        return self.create_login_url()

    def authenticate_user(self):
        credential = pickle.loads(self.session.get('credential'))
        if credential.access_token_expired:
            try:
                credential.refresh(httplib2.Http())
            except AccessTokenRefreshError:
                self.response.set_status(401)
                self.response.write('Unauthorized Access - Credential refresh failed')
                self.logout()
                return False
            else:
                self.session['credential'] = pickle.dumps(credential)
        return True