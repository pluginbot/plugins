QT       += core
QT	 += network

TEMPLATE = lib
TARGET = TelnetSvr
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
    telnetclient.h \
    telnetserver.h \
    telnetsvr.h

SOURCES += \
    telnetclient.cpp \
    telnetserver.cpp \
    telnetsvr.cpp

