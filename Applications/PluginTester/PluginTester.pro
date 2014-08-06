#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T21:21:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PluginTester
TEMPLATE = app

#Include the config files and set the destination dir
include(../../config.pri)
DESTDIR =$${DISTDIR}

#Include the interface
INCLUDEPATH += $$quote($$INTERFACEDIR)
INCLUDEDIR += $$quote($$INTERFACEDIR)
include($$quote($$INTERFACEDIR/Interfaces.pri))

SOURCES += main.cpp\
        mainwindow.cpp \
    frmupdate.cpp \
    frmproperties.cpp

HEADERS  += mainwindow.h \
    frmupdate.h \
    frmproperties.h

FORMS    += mainwindow.ui \
    frmupdate.ui \
    frmproperties.ui

RESOURCES += \
    main.qrc
