import json

from ValidateCredential import ValidateCredential
from handlers.BaseHandler import BaseHandler
from models.GoogleDocsChange import GoogleDocsChange
from push import RetrieveChange


class GetChangesHandler(BaseHandler):
    @ValidateCredential
    def post(self):
        user_id = self.session['user_id']
        entity = GoogleDocsChange.get_or_insert('user_id', user_id=user_id)
        changes = list(entity.change_ids)
        entity.change_ids[:] = []
        entity.save()

        response = []
        for change_id in changes:
            res = RetrieveChange(self.drive_service, change_id)
            response.append(res)

        self.response.content_type = 'application/json'
        self.response.write(json.dumps({'changes_list': response}))
        self.response.set_status(200)