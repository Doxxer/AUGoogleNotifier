TEMPLATE = app

QT += core widgets network
CONFIG += debug_and_release

CONFIG (debug, debug | release) {
    TARGET = client_debug
    OBJECTS_DIR = build/obj/debug
    RCC_DIR = build/rcc/debug
} else {
    TARGET = GoogleNotifier
    OBJECTS_DIR = build/obj/release
    RCC_DIR = build/rcc/release
}
MOC_DIR = build/moc
DESTDIR = bin

macx {
    QMAKE_INFO_PLIST = resources/info.plist
    ICON = resources/icon.icns
}

RESOURCES = Client.qrc

VPATH += src src/core src/console src/gui
INCLUDEPATH += src/core src/console src/gui

HEADERS += Authorizer.hpp \
           AuthorizationChecker.hpp \
           ChangesGetter.hpp \
           ChangesPoller.hpp \
           Controller.hpp \
           CommandProcessor.hpp \
           CookieJar.hpp \
           Logouter.hpp \
           NetworkManager.hpp \
           Notifier.hpp \
           OneShotHttpServer.hpp \
           PollingManager.hpp \
           Requester.hpp \
           Resubscriber.hpp \
           Subscriber.hpp \
           Unsubscriber.hpp \
           UserDialog.hpp
SOURCES += Authorizer.cpp \
           AuthorizationChecker.cpp \
           ChangesGetter.cpp \
           ChangesPoller.cpp \
           Controller.cpp \
           CookieJar.cpp \
           Logouter.cpp \
           main.cpp \
           NetworkManager.cpp \
           Notifier.cpp \
           OneShotHttpServer.cpp \
           PollingManager.cpp \
           Requester.cpp \
           Resubscriber.cpp \
           Subscriber.cpp \
           Unsubscriber.cpp \
           UserDialog.cpp
