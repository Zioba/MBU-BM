#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T18:37:03
#
#-------------------------------------------------

QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MBY
TEMPLATE = app


SOURCES += main.cpp\
        deldialog.cpp \
        mainwindow.cpp \
    ipdialog.cpp \
    logger.cpp \
    converter.cpp

HEADERS  += mainwindow.h\
    deldialog.h \
    ipdialog.h \
    logger.h \
    converter.h

FORMS    += mainwindow.ui\
    deldialog.ui \
    ipdialog.ui
