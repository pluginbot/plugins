QT       += core
QT       += xml

TEMPLATE = lib
TARGET = WindowsKeyLogger
CONFIG += plugin
VERSION = 1.0.0

#Include the config files and set the destination
include(../../config.pri)
DESTDIR =$${DISTPLUGINDIR}

#Include the interface
INCLUDEPATH += $$quote($$INTERFACEDIR)
INCLUDEDIR += $$quote($$INTERFACEDIR)
include($$quote($$INTERFACEDIR/Interfaces.pri))


HEADERS += \
    windowskeylogger.h \
    keylogger.h \
    logevent.h \
    logprocess.h \
    singleton.h \
    win32keyhook.h \
    windowsinfo.h

SOURCES += \
    windowskeylogger.cpp \
    keylogger.cpp \
    logevent.cpp \
    logprocess.cpp \
    win32keyhook.cpp \
    windowsinfo.cpp

