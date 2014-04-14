import os

import jinja2
import webapp2

from handlers.GoogleVerificationHandler import GoogleVerificationHandler

from handlers.MainPageHandler import MainPageHandler

JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.join(os.path.dirname(__file__), 'templates')),
    extensions=['jinja2.ext.autoescape'])

handler = webapp2.WSGIApplication([
                                      ('/', MainPageHandler),
                                      ('/googled2ac93807b24db11.html', GoogleVerificationHandler),
                                  ], debug=True)