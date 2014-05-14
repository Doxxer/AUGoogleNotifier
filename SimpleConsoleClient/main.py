#!/usr/bin/env python
# coding=utf-8
import json

from CookieManager import load_cookies, save_cookies
from OneRequestWebServer import grab_authorization_code
import requests

# APPSERVER = "http://localhost:10080"
APPSERVER = "https://spbau-notifier-583.appspot.com"

LOGIN_ADDRESS = APPSERVER + "/login"
LOGOUT_ADDRESS = APPSERVER + "/logout"
SUBSCRIBE_ADDRESS = APPSERVER + "/subscribe"
UNSUBSCRIBE_ADDRESS = APPSERVER + "/unsubscribe"
AUTH_CALLBACK_ADDRESS = APPSERVER + "/oauth2callback"
GET_CHANGES_ADDRESS = APPSERVER + "/get_changes"
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


def make_subscribe_request():
    response = SESSION.post(SUBSCRIBE_ADDRESS)
    print response.status_code, response.reason, response.text


def make_unsubscribe_request():
    response = SESSION.post(UNSUBSCRIBE_ADDRESS)
    print response.status_code, response.reason, response.text


def make_get_changes_request():
    response = SESSION.post(GET_CHANGES_ADDRESS).text
    response = json.loads(response)['changes_list']

    if not response:
        print "no changes"
    else:
        for k, change in enumerate(response):
            print "--------- ", k
            assert isinstance(change, dict)
            for k in change.keys():
                print k, change[k]


def process_message(message):
    if message.strip().lower() == 'a':
        make_auth_request()
    elif message.strip().lower() == 'l':
        make_logout_request()
    elif message.strip().lower() == 's':
        make_subscribe_request()
    elif message.strip().lower() == 'g':
        make_get_changes_request()
    elif message.strip().lower() == 'u':
        make_unsubscribe_request()
    elif message.strip().lower() == 'q':
        return False
    else:
        show_usage()
    return True


def show_usage():
    print "Usage: a | l | s | g | u | q"
    print "a -- try to authenticate. If success it returns \"OK %USERNAME%\""
    print "l -- logout"
    print "s -- Подписаться на изменения. Нужно быть авторизованным"
    print "g -- получить все изменения с сервера. Нужно быть авторизованным"
    print "u -- отписаться от изменений. Нужно быть авторизованным"
    print "q -- quit"


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