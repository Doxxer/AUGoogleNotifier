import cookielib
import os
from uuid import getnode as get_mac

def load_cookies(cookies, filename):
    if not os.path.isfile(filename):
        save_cookies(cookies, filename)

    lwp_cookiejar = cookielib.LWPCookieJar()
    lwp_cookiejar.load(filename, ignore_discard=True)

    c = cookielib.Cookie(version=0, name='mac_address', value=str(get_mac()), port=None, port_specified=False,
                         domain="", domain_specified=False, domain_initial_dot=None, path="/",
                         path_specified=False, secure=False, expires=None, discard=True,
                         comment=None, comment_url=None, rest=None)
    lwp_cookiejar.set_cookie(c)

    return lwp_cookiejar


def save_cookies(cookiejar, filename):
    lwp_cookiejar = cookielib.LWPCookieJar()
    for c in cookiejar:
        try:
            args = dict(vars(c).items())
            if args['name'] == 'mac_address':
                continue
            args['rest'] = args['_rest']
            del args['_rest']
            c = cookielib.Cookie(**args)
            lwp_cookiejar.set_cookie(c)
        except TypeError:
            pass
    lwp_cookiejar.save(filename, ignore_discard=True)