import hashlib
import os

import jinja2
import webapp2

from oauth2client.clientsecrets import loadfile

from oauth2client.client import flow_from_clientsecrets

from handlers.GoogleVerificationHandler import GoogleVerificationHandler

from handlers.LoginHandler import LoginHandler
from handlers.LogoutHandler import LogoutHandler

from handlers.MainPageHandler import MainPageHandler
from handlers.OAuth2CallbackHandler import OAuth2CallbackHandler


JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.join(os.path.dirname(__file__), 'templates')),
    extensions=['jinja2.ext.autoescape'])

CLIENT_SECRETS = os.path.join(os.path.dirname(__file__), 'client_secrets_installed.json')
client_type, client_info = loadfile(CLIENT_SECRETS)
FLOW = flow_from_clientsecrets(CLIENT_SECRETS,
                               scope=('https://www.googleapis.com/auth/drive ',
                                      'https://www.googleapis.com/auth/userinfo.profile'),
                               redirect_uri=client_info['redirect_uris'][0], )
FLOW.params.update({'access_type': 'offline'})
FLOW.params.update({'approval_prompt': 'force'})

webApp2Config = {'webapp2_extras.sessions': {'secret_key': hashlib.sha512(FLOW.client_secret).hexdigest()}}


# Configure url handlers for webapp2
handler = webapp2.WSGIApplication([
                                      ('/', MainPageHandler),
                                      ('/googled2ac93807b24db11.html', GoogleVerificationHandler),
                                      ('/oauth2callback', OAuth2CallbackHandler),
                                      ('/login', LoginHandler),
                                      ('/logout', LogoutHandler),
                                  ], config=webApp2Config, debug=True)