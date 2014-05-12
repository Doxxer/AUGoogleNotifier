TEMPLATE = app

QT += core network
QT -= gui
CONFIG += debug_and_release console

TARGET = client

OBJECTS_DIR = bin
MOC_DIR = moc
DESTDIR = .
VPATH += src

HEADERS += Authorizer.hpp \
           CommandProcessor.hpp \
           CookieJar.hpp \
           NetworkManager.hpp \
           UserDialog.hpp
SOURCES += Authorizer.cpp \
           CookieJar.cpp \
           main.cpp \
           NetworkManager.cpp \
           UserDialog.cpp
