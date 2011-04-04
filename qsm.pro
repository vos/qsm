#-------------------------------------------------
#
# Project created by QtCreator 2011-01-15T13:50:11
#
#-------------------------------------------------

QT += core gui opengl xml

TARGET = qsm
TEMPLATE = app


HEADERS += qsm.h \
           mainwindow.h \
           scanfolderthread.h \
           imageinfo.h \
           imageloader.h \
           imageloaderthread.h \
           imageloaderpool.h \
           imagelistmodel.h \
           imagewidget.h \
           slideshowimage.h \
           slideshow.h \
           slideshowlistmodel.h

SOURCES += main.cpp \
           mainwindow.cpp \
           scanfolderthread.cpp \
           imageinfo.cpp \
           imageloader.cpp \
           imageloaderthread.cpp \
           imageloaderpool.cpp \
           imagelistmodel.cpp \
           imagewidget.cpp \
           slideshowimage.cpp \
           slideshow.cpp \
           slideshowlistmodel.cpp

FORMS += mainwindow.ui

RESOURCES += icons.qrc

OTHER_FILES += README.md


TRANSLATIONS = qsm_de.ts
CODECFORTR = ISO-8859-15
CODECFORSRC = UTF-8
