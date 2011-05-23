#-------------------------------------------------
#
# Project created by QtCreator 2011-01-15T13:50:11
#
#-------------------------------------------------

QT += core gui opengl

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
           imagelistview.h \
           imagewidget.h \
           slideshowimage.h \
           slideshow.h \
           slideshowlistmodel.h \
           slideshowlistview.h \
           slideshowfilemanager.h \
           slideshowwindow.h \
           optionsdialog.h

SOURCES += main.cpp \
           mainwindow.cpp \
           scanfolderthread.cpp \
           imageinfo.cpp \
           imageloader.cpp \
           imageloaderthread.cpp \
           imageloaderpool.cpp \
           imagelistmodel.cpp \
           imagelistview.cpp \
           imagewidget.cpp \
           slideshowimage.cpp \
           slideshow.cpp \
           slideshowlistmodel.cpp \
           slideshowlistview.cpp \
           slideshowfilemanager.cpp \
           slideshowwindow.cpp \
           optionsdialog.cpp

FORMS += mainwindow.ui \
         slideshowwindow.ui \
         optionsdialog.ui

RESOURCES += icons.qrc

#RC_FILE = qsm.rc

OTHER_FILES += README.md \
               qsm_de.ts \
               qsm.rc


TRANSLATIONS = qsm_de.ts
CODECFORTR = ISO-8859-15
CODECFORSRC = UTF-8
