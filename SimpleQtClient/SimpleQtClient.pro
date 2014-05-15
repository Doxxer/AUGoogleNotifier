TEMPLATE = app

QT += core gui widgets network
CONFIG += debug_and_release

TARGET = client

OBJECTS_DIR = bin
MOC_DIR = moc
DESTDIR = .
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
