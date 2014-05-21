import logging
import pickle
from httplib import HTTPException
import urllib
import uuid

from google.appengine.runtime.apiproxy_errors import DeadlineExceededError
import webapp2
from webapp2_extras import sessions

from apiclient import errors
from handlers.Errors import CookieError
from push import stop_channel
import view
from push import watch_change



# noinspection PyProtectedMember,PyAttributeOutsideInit
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
            self.unsubscribe(force_delete=True)
            for k in list(self.session.keys()):
                del self.session[k]

    def subscribe(self):
        """
        Subscribe to push notifications
        Returns:
            dict, containing result of unsubscribe request.
            dict['success'], boolean, True if unsubscribe request was successful.
            Additional following data when unsubscribe request failed.
            dict['error_code'], int, Status code of unsubscribe request.
            dict['error_msg'], string, Error message of unsubscribe request.
        """
        result = {}

        notification_id = str(uuid.uuid4())
        channel_type = 'web_hook'
        address = self.request.host_url + '/notificationcallback'
        params = {'ttl': 1800}
        token_data = {'user_id': self.session['user_id']}
        token_string = urllib.urlencode(token_data)
        credential = pickle.loads(self.session.get('credential'))
        channel = None

        if not credential.access_token_expired and hasattr(self, 'drive_service'):
            try:
                channel = watch_change(self.drive_service,
                                       notification_id,
                                       channel_type,
                                       address,
                                       channel_params=params,
                                       channel_token=token_string)

            except errors.HttpError, error:
                logging.warning('HttpError occurred while subscribing push notifications')
                logging.warning(error)
                result['success'] = False
                result['error_code'] = error.resp.status
                result['error_msg'] = error._get_reason().strip()
            except (DeadlineExceededError, HTTPException), error:
                logging.warning('DeadlineExceededError/HTTPException occurred while subscribing push notifications')
                logging.warning(error)
                result['success'] = False
                result['error_code'] = 504
                result['error_msg'] = "Request exceeded its deadline. Please try again"
            else:
                result['success'] = True
        else:
            result['success'] = False
            result['error_code'] = 401
            result['error_msg'] = 'Credential expired'

        if result['success']:
            self.session['notification_id'] = channel['id']
            self.session['resource_id'] = channel['resourceId']

        return result

    def unsubscribe(self, force_delete=False):
        """Unsubscribe from push notifications.

        :param force_delete: boolean, force delete session even when unsubscribe fails.
        Returns:
            dict, containing result of unsubscribe request.
            dict['success'], boolean, True if unsubscribe request was successful.
            Additional following data when unsubscribe request failed.
            dict['error_code'], int, Status code of unsubscribe request.
            dict['error_msg'], string, Error message of unsubscribe request.
        """
        result = {}
        delete_anyway = False

        # Retrieve task-specific notification_id and resource_id
        notification_id = self.session.get('notification_id')
        resource_id = self.session.get('resource_id')

        # If not subscribed, return
        if not notification_id or not resource_id:
            result['success'] = False
            result['error_code'] = 400
            result['error_msg'] = 'Not subscribed'
            return result

        # Make unsubscribe request
        credential = pickle.loads(self.session.get('credential'))
        if not credential.access_token_expired and hasattr(self, 'drive_service'):
            try:
                stop_channel(self.drive_service, notification_id, resource_id)
            except errors.HttpError, error:
                logging.warning('HttpError occurred while unsubscribing push notifications')
                logging.warning(error)
                result['success'] = False
                result['error_code'] = error.resp.status
                result['error_msg'] = error._get_reason().strip()
                delete_anyway = True
            except (DeadlineExceededError, HTTPException), error:
                logging.warning('DeadlineExceededError/HTTPException occurred while subscribing push notifications')
                logging.warning(error)
                result['success'] = False
                result['error_code'] = 504
                result['error_msg'] = ('Request exceeded its deadline.'
                                       'Please try again')
            else:
                result['success'] = True
        else:
            result['success'] = False
            result['error_code'] = 401
            result['error_msg'] = 'Credential expired'

        if result['success'] or force_delete or delete_anyway:
            del self.session['notification_id']
            del self.session['resource_id']
        return result

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
        if 'Cookie' not in self.request.headers:
            raise CookieError()
        cookie_dict = dict(item.split("=") for item in self.request.headers['Cookie'].split(";"))
        return cookie_dict.get(key)