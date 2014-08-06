#-------------------------------------------------
#
# Project created by QtCreator 2014-03-06T19:28:05
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PluginGenerator
TEMPLATE = app

#Include the config files and set the destination dir
include(../../config.pri)
DESTDIR =$${DISTDIR}

#Include the interface
INCLUDEPATH += $$quote($$INTERFACEDIR)
INCLUDEDIR += $$quote($$INTERFACEDIR)
include($$quote($$INTERFACEDIR/Interfaces.pri))

#Add the zip lib
INCLUDEDIR += $$quote($$LIBDIR/Zip/)
INCLUDEPATH += $$quote($$LIBDIR/Zip/)
DEPENDPATH += $$quote($$LIBDIR/Zip/)

SOURCES += main.cpp\
        dialog.cpp \
    frmabout.cpp \
    operatingsystem.cpp

HEADERS  += dialog.h \
            $$quote($$LIBDIR/Zip/zipwriter.h) \
    frmabout.h \
    operatingsystem.h

FORMS    += dialog.ui \
    frmabout.ui

#Add the libs
unix {
LIBS += -L$${DISTDIR} -lZip
}

win32 {
LIBS += -L$${DISTDIR} -lZip1
}



