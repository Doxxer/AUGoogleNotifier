import logging

import webapp2
from webapp2_extras import sessions

import view


class BaseHandler(webapp2.RequestHandler):
    def dispatch(self):
        """
        Get a session store for this request
        """
        self.session_store = sessions.get_store(request=self.request)
        try:
            webapp2.RequestHandler.dispatch(self)
        finally:
            self.session_store.save_sessions(self.response)

    def render(self, template):
        jinja_template = view.JINJA_ENVIRONMENT.get_template(template)
        self.response.write(jinja_template.render())

    @webapp2.cached_property
    def session(self):
        """
        Return a session using key from the configuration
        """
        return self.session_store.get_session(backend='datastore')

    def logout(self):
        """
        Delete credential in session data
        """
        for k in list(self.session.keys()):
            del self.session[k]

    @staticmethod
    def create_login_url():
        """
        Return an oauth authorization url if user needs authorization.
        """
        return view.FLOW.step1_get_authorize_url()

    def get_value_from_cookie(self, key):
        """
        Return a value by key from cookie_header
        """
        cookie_dict = dict(item.split("=") for item in self.request.headers['Cookie'].split(";"))
        return cookie_dict.get(key)