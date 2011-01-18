#-------------------------------------------------
#
# Project created by QtCreator 2011-01-15T13:50:11
#
#-------------------------------------------------

QT += core gui xml opengl

TARGET = qsm
TEMPLATE = app


HEADERS += mainwindow.h \
           scanfolderthread.h \
           imagelistmodel.h \
           imagewidget.h

SOURCES += main.cpp \
           mainwindow.cpp \
           scanfolderthread.cpp \
           imagelistmodel.cpp \
           imagewidget.cpp

FORMS += mainwindow.ui


TRANSLATIONS = qsm_de.ts
CODECFORTR = ISO-8859-15
CODECFORSRC = UTF-8
