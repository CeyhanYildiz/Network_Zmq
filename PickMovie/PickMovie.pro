TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lnzmqt -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += main.cpp \
    moviepicker.cpp \
    userhandler.cpp

HEADERS += \
    moviepicker.h \
    userhandler.h
