#!/usr/bin/env python

from CookiesHelper import load_cookies, save_cookies

from OneRequestWebServer import grab_authorization_code
import requests

# APPSERVER = "http://localhost:9080"
APPSERVER = "https://spbau-notifier-583.appspot.com"

LOGIN_ADDRESS = APPSERVER + "/login"
LOGOUT_ADDRESS = APPSERVER + "/logout"
AUTH_CALLBACK_ADDRESS = APPSERVER + "/oauth2callback"
COOKIES_FILE = 'cookies.txt'
SESSION = requests.Session()


def make_auth_request():
    response = SESSION.post(LOGIN_ADDRESS).text

    if response.split()[0].strip().lower() == "ok":
        print response
    else:
        auth_code = grab_authorization_code(url=response)
        response = SESSION.get(AUTH_CALLBACK_ADDRESS, params={'code': auth_code})
        print response.status_code, response.reason, response.text


def make_logout_request():
    response = SESSION.get(LOGOUT_ADDRESS)
    print response.status_code, response.reason, response.text


def process_message(message):
    if message.strip().lower() == 'auth':
        make_auth_request()
    elif message.strip().lower() == 'logout':
        make_logout_request()
    elif message.strip().lower() == 'q':
        return False
    else:
        show_usage()
    return True

def show_usage():
    print "Usage: auth | logout | q"
    print "auth -- try to authenticate. If success it returns \"OK %USERNAME%\". Else -- URL"
    print "logout -- logout"


def main():
    show_usage()
    try:
        while process_message(raw_input('your message: ')):
            pass
    except (KeyboardInterrupt, EOFError):
        print


if __name__ == "__main__":
    SESSION.cookies = load_cookies(SESSION.cookies, COOKIES_FILE)
    main()
    save_cookies(SESSION.cookies, COOKIES_FILE)