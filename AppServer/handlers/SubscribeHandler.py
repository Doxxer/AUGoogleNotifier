from httplib import HTTPException
import json
import logging
import urllib
import uuid

from google.appengine.runtime.apiproxy_errors import DeadlineExceededError

from apiclient import errors
from ValidateCredential import ValidateCredential
from handlers.BaseHandler import BaseHandler
from push import WatchChange


class SubscribeHandler(BaseHandler):
    @ValidateCredential
    def post(self):
        """
        POST request handling method.

        Make POST request to subscribe for Drive Files/Changes resources.
        """
        if self.session.get('notification_id'):
            # todo renew
            self.response.set_status(200)
            self.response.write("renewed notification channel")
            return

        notification_id = str(uuid.uuid4())
        channel_type = 'web_hook'
        address = self.request.host_url + '/notificationcallback'
        params = {'ttl': 1800}
        token_data = {'user_id': self.session['user_id']}
        token_string = urllib.urlencode(token_data)
        try:
            result = WatchChange(self.drive_service, notification_id, channel_type,
                                 address, channel_params=params, channel_token=token_string)

        except errors.HttpError, error:
            self.response.set_status(error.resp.status)
            self.response.write(error._get_reason().strip())
        except (DeadlineExceededError, HTTPException):
            self.response.set_status(510)
            self.response.write('Request exceeded its deadline. Please try again')
        else:
            logging.warning("Subscribed ok")
            self.session['notification_id'] = result['id']
            self.response.set_status(200)