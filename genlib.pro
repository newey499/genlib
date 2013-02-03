#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T16:17:30
#
#-------------------------------------------------

DEFINES += DEBUG

QT += core
QT += sql
QT += gui
QT += xml
QT += network

TARGET = genlib
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    database.cpp \
    main.cpp \
    sbqtableview.cpp \
    sbrowedit.cpp \
    sbqrelationaltablemodel.cpp \
    sbrelationcombobox.cpp \
    mainwindow.cpp \
		curl.cpp \
		sbrowse.cpp

HEADERS += \
    database.h \
    sbqtableview.h \
    sbrowedit.h \
    sbqrelationaltablemodel.h \
    sbrelationcombobox.h \
    mainwindow.h \
		curl.h \
		sbrowse.h

FORMS += \
    mainwindow.ui
