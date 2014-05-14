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
           CookieJar.hpp \
           Logouter.hpp \
           NetworkManager.hpp \
           OneShotHttpServer.hpp \
           Subscriber.hpp \
           UserDialog.hpp
SOURCES += Authorizer.cpp \
           CookieJar.cpp \
           Logouter.cpp \
           main.cpp \
           NetworkManager.cpp \
           OneShotHttpServer.cpp \
           Subscriber.cpp \
           UserDialog.cpp
