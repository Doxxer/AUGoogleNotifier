import json
import logging
import urlparse

from handlers.BaseHandler import BaseHandler
from models.GoogleDocsChange import GoogleDocsChange


class NotificationCallbackHandler(BaseHandler):
    """
    Request handling class for /notification
    """

    def post(self):
        """
        POST request handling method.

        Stores incoming changes using GAE datastore API
        """

        # Ignore sync message.
        if self.request.headers['X-Goog-Resource-State'] == 'sync':
            return

        # Receive user_id and change_id
        user_id = dict(urlparse.parse_qsl(self.request.headers['X-Goog-Channel-Token']))['user_id']
        change_id = json.loads(self.request.body)['id']

        logging.warning(user_id)
        logging.warning(change_id)

        entity = GoogleDocsChange.get_or_insert(str(user_id), user_id=user_id)
        if change_id not in entity.change_ids:
            entity.change_ids.append(change_id)
        entity.save()
        self.response.set_status(200)