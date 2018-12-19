#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T09:36:06
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Danmu
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Danmu.cpp \
    add_to_bl.cpp \
    delete_from_bl.cpp \
    logindialog.cpp \
    registerdialog.cpp

HEADERS  += mainwindow.h \
    Danmu.h \
    add_to_bl.h \
    delete_from_bl.h \
    logindialog.h \
    registerdialog.h

FORMS    += mainwindow.ui \
    add_to_bl.ui \
    delete_from_bl.ui \
    logindialog.ui \
    registerdialog.ui
