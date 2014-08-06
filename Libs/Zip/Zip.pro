#-------------------------------------------------
#
# Zip is a wrapper for the zLib library
#
#-------------------------------------------------

TEMPLATE = lib
TARGET = Zip
CONFIG += plugin release
VERSION = 1.0.0
DEFINES += ZIP_LIBRARY

#include the config file
include(../../config.pri)
include($$PWD/zipconfig.pri)

#Send the compiled binraies to the BUILDDIR so we can link to them easily
DESTDIR =$${DISTDIR}

HEADERS += \
    zipreader.h \
    zipwriter.h \
    zlib.h \
    zconf.h

SOURCES += \
    zip.cpp
