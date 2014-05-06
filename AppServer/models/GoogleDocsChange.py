from google.appengine.ext import db


class GoogleDocsChange(db.Model):
    user_id = db.StringProperty(required=True)
    change_ids = db.StringListProperty()