from OneRequestWebServer import grab_authorization_code
import requests

# APPSERVER = "http://localhost:9080"
APPSERVER = "https://spbau-notifier-583.appspot.com"

LOGIN_ADDRESS = APPSERVER + "/login"
LOGOUT_ADDRESS = APPSERVER + "/logout"
AUTH_CALLBACK_ADDRESS = APPSERVER + "/oauth2callback"


def make_auth_request():
    request = requests.post(LOGIN_ADDRESS).text

    print request

    if request.split()[0].strip().lower() == "ok":
        print request
    else:
        auth_code = grab_authorization_code(request)
        request = requests.get(AUTH_CALLBACK_ADDRESS, params={'code': auth_code})
        print request.status_code, request.reason, request.text


def make_logout_request():
    request = requests.get(LOGOUT_ADDRESS)
    print request.status_code, request.reason, request.text


def process_message(message):
    if message.strip().lower() == 'auth':
        make_auth_request()
    elif message.strip().lower() == 'logout':
        make_logout_request()
    else:
        show_usage()


def show_usage():
    print "Usage: auth | logout "
    print "auth -- try to authenticate. If success it returns \"OK %USERNAME%\". Else -- URL"
    print "logout -- logout"


def main():
    show_usage()
    try:
        while True:
            process_message(raw_input('your message: '))
    except (KeyboardInterrupt, EOFError):
        print


if __name__ == "__main__":
    main()