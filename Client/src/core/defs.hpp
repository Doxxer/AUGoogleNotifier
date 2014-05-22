#ifndef DEFS_HPP
#define DEFS_HPP


char const * const ORG_NAME = "SPbAU";
char const * const ORG_DOMAIN = "mit.spbau.ru";
char const * const APP_NAME = "AUGoogleNotifier";


char const * const APPSERVER = "https://spbau-notifier-583.appspot.com/";
char const * const LOGIN_PATH = "login";
char const * const LOGOUT_PATH = "logout";
char const * const SUBSCRIBE_PATH = "subscribe";
char const * const UNSUBSCRIBE_PATH = "unsubscribe";
char const * const CHANGES_PATH = "get_changes";
char const * const AUTH_CALLBACK_PATH = "oauth2callback";

char const * const RECENT_URL = "https://drive.google.com/#recent";

char const * const SUBSCRIBED_SETTING = "subscribed";

int const HTTP_PORT = 19004;

int const RESUBSCRIBE_TIMEOUT = 25 * 60 * 1000;
int const CHANGES_TIMEOUT = 30 * 1000;


#endif // DEFS_HPP
