import json
import logging

from validateCredential import validateCredential
from handlers.BaseHandler import BaseHandler
from models.GoogleDocsChange import GoogleDocsChange
from push import retrieve_change


class GetChangesHandler(BaseHandler):
    @validateCredential
    def post(self):
        user_id = self.session['user_id']
        entity = GoogleDocsChange.get_or_insert(str(user_id), user_id=user_id)
        changes = list(entity.change_ids)
        entity.change_ids[:] = []
        entity.save()

        logging.debug("User id retrieving changes: {0}".format(str(user_id)))
        logging.debug("Changes:")
        logging.debug(changes)

        response = []
        for change_id in changes:
            change = retrieve_change(self.drive_service, change_id)
            if change:
                response.append(change)

        self.response.content_type = 'application/json'
        self.response.write(json.dumps({'changes_list': response}))
        self.response.set_status(200)