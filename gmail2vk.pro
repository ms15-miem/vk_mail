#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T12:25:19
#
#-------------------------------------------------

QT       += core

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gmail2vk
TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    oauth.cpp \
    gmail.cpp \
    vk.cpp \
    message.cpp \
    gmailmessage.cpp

HEADERS  += \
    client.h \
    oauth.h \
    gmail.h \
    vk.h \
    message.h \
    gmailmessage.h

FORMS    +=


QT += network xml

HEADERS += \
    vkmessage.h

SOURCES += \
    vkmessage.cpp

