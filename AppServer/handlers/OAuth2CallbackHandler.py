import pickle

import httplib2

import view
from apiclient.discovery import build
from oauth2client.client import FlowExchangeError
from handlers.BaseHandler import BaseHandler


class OAuth2CallbackHandler(BaseHandler):
    """
    Request handling class for /oauth2callback
    """

    def get(self):
        """
        GET request handling method.

        Receive authentication code from user with GET['code'].
        Save credential if code exchange is successful.
        """
        if 'code' not in self.request.GET:
            self.response.set_status(400)
            self.response.write("Authorization code not found")
            return

        try:
            credential = view.FLOW.step2_exchange(self.request.GET.get('code'))
        except FlowExchangeError:
            pass
        else:
            # Retrieve basic information about the user
            users_service = build('oauth2', 'v2', http=credential.authorize(httplib2.Http()))
            user_information = users_service.userinfo().get().execute()

            self.session['credential'] = pickle.dumps(credential)
            self.session['mac_address'] = self.get_value_from_cookie('mac_address')
            self.session['user_id'] = user_information['id']
            self.session['name'] = user_information['name']
            self.response.set_status(200)
            self.response.write("Authentication successful. Your name is " + self.session['name'])