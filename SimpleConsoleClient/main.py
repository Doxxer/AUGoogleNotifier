import cookielib

from OneRequestWebServer import grab_authorization_code
import requests


# APPSERVER = "http://localhost:9080"
APPSERVER = "https://spbau-notifier-583.appspot.com"

LOGIN_ADDRESS = APPSERVER + "/login"
LOGOUT_ADDRESS = APPSERVER + "/logout"
AUTH_CALLBACK_ADDRESS = APPSERVER + "/oauth2callback"

COOKIES = 'cookies.txt'

session = requests.Session()


def make_auth_request():
    #s = requests.Session()

    response = session.post(LOGIN_ADDRESS).text

    if response.split()[0].strip().lower() == "ok":
        print response
    else:
        auth_code = grab_authorization_code(url=response)
        response = session.get(AUTH_CALLBACK_ADDRESS, params={'code': auth_code})
        print response.status_code, response.reason, response.text


def make_logout_request():
    response = session.get(LOGOUT_ADDRESS)
    print response.status_code, response.reason, response.text


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


def load_cookies_from_lwp(filename):
    lwp_cookiejar = cookielib.LWPCookieJar()
    lwp_cookiejar.load(filename, ignore_discard=True)
    return lwp_cookiejar


def save_cookies_lwp(cookiejar, filename):
    lwp_cookiejar = cookielib.LWPCookieJar()
    for c in cookiejar:
        args = dict(vars(c).items())
        args['rest'] = args['_rest']
        del args['_rest']
        c = cookielib.Cookie(**args)
        lwp_cookiejar.set_cookie(c)
    lwp_cookiejar.save(filename, ignore_discard=True)


if __name__ == "__main__":
    session.cookies = load_cookies_from_lwp(COOKIES)
    main()
    save_cookies_lwp(session.cookies, COOKIES)