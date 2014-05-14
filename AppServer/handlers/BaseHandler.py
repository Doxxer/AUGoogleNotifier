import logging
import pickle
from httplib import HTTPException

from google.appengine.runtime.apiproxy_errors import DeadlineExceededError
import webapp2
from webapp2_extras import sessions

from apiclient import errors
from push import StopChannel
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
        if 'credential' in self.session:
            self.Unsubscribe(force_delete=True)
            for k in list(self.session.keys()):
                del self.session[k]

    def Unsubscribe(self, force_delete=False):
        """Unsubscribe from push notifications.

        Args:
          force_delete: boolean, force delete session even when unsubscribe fails.
        Returns:
          dict, containing result of unsubscribe request.
          dict['success'], boolean, True if unsubscribe request was successful.
          Additional following data when unsubscribe request failed.
          dict['error_code'], int, Status code of unsubscribe request.
          dict['error_msg'], string, Error message of unsubscribe request.
        """
        return_val = {}

        # Retrieve task-specific notification_id and resource_id
        notification_id = self.session.get('notification_id')
        resource_id = self.session.get('resource_id')

        # If not subscribed, return

        if not notification_id:
            return_val['success'] = False
            return_val['error_code'] = 400
            return_val['error_msg'] = 'Not subscribed'
            return return_val

        # Make unsubscribe request
        credential = pickle.loads(self.session.get('credential'))
        if not credential.access_token_expired and hasattr(self, 'drive_service'):
            try:
                StopChannel(self.drive_service, notification_id, resource_id)
            except errors.HttpError, error:
                logging.error(error)
                return_val['success'] = False
                return_val['error_code'] = error.resp.status
                return_val['error_msg'] = error._get_reason().strip()
            except (DeadlineExceededError, HTTPException):
                return_val['success'] = False
                return_val['error_code'] = 510
                return_val['error_msg'] = ('Request exceeded its deadline.'
                                           'Please try again')
            else:
                return_val['success'] = True
        else:
            return_val['success'] = False
            return_val['error_code'] = 401
            return_val['error_msg'] = 'Credential expired'

        if return_val['success'] or force_delete:
            del self.session['notification_id']
            del self.session['resource_id']
        return return_val

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