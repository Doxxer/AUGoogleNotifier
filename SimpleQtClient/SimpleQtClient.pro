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
           GetRequester.hpp \
           OneShotHttpServer.hpp \
           NetworkManager.hpp \
           UserDialog.hpp
SOURCES += Authorizer.cpp \
           CookieJar.cpp \
           GetRequester.cpp \
           OneShotHttpServer.cpp \
           main.cpp \
           NetworkManager.cpp \
           UserDialog.cpp
