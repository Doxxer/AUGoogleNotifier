TEMPLATE = app

QT += core widgets network
CONFIG += debug_and_release

CONFIG (debug, debug | release) {
    TARGET = client_debug
    OBJECTS_DIR = obj/debug
} else {
    TARGET = client
    OBJECTS_DIR = obj/release
}
MOC_DIR = moc
DESTDIR = bin
VPATH += src

HEADERS += Authorizer.hpp \
           CommandProcessor.hpp \
           ChangesGetter.hpp \
           CookieJar.hpp \
           Logouter.hpp \
           NetworkManager.hpp \
           OneShotHttpServer.hpp \
           Requester.hpp \
           Subscriber.hpp \
           Unsubscriber.hpp \
           UserDialog.hpp
SOURCES += Authorizer.cpp \
           ChangesGetter.cpp \
           CookieJar.cpp \
           Logouter.cpp \
           main.cpp \
           NetworkManager.cpp \
           OneShotHttpServer.cpp \
           Requester.cpp \
           Subscriber.cpp \
           Unsubscriber.cpp \
           UserDialog.cpp