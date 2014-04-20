import pickle
from apiclient.discovery import build

from oauth2client.client import AccessTokenRefreshError

import httplib2


def ValidateCredential(function):
    """
    Decorator to validate credentials
    """

    def _decorated(self, *args, **kwargs):
        if 'credential' in self.session:
            # Load credential from session data.
            credential = pickle.loads(self.session.get('credential'))
            http = httplib2.Http()
            if credential.access_token_expired:
                try:
                    credential.refresh(http)
                except AccessTokenRefreshError:
                    # When credential is invalid and refreshing fails, it returns 401.
                    self.response.set_status(401)
                    self.response.write('Unauthorized Access - Credential refresh failed')
                    self.logout()
                    return
                else:
                    self.session['credential'] = pickle.dumps(credential)
            self.drive_service = build('drive', 'v2', http=credential.authorize(http))
            return function(self, *args, **kwargs)
        else:
            self.response.set_status(401)
            self.response.write('Unauthorized Access - User not logged in')

    return _decorated
