/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <alexander.vos@stud.fh-swf.de>,
**                    Till Althaus <till.althaus@stud.fh-swf.de>
**
** This file is part of Qt Slideshow Manager (QSM).
**
** QSM is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** QSM is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with QSM.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>

#include "mainwindow.h"

/*!
  \brief The main entry point of this application.

  The main function will install a global translator for internationalization
  to support other languages than english, the function will then show the
  MainWindow GUI to the user and start the main event loop for event handling.

  \param argc The command-line argument count.
  \param argv The command-line argument-array.
  \return The return code of the application.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("FH-SWF");
    app.setOrganizationDomain("fh-swf.de");
    app.setApplicationName("Qt Slideshow Manager");

    QSettings settings;
    QLocale::Language language = (QLocale::Language)settings.value("language",
        (int)QLocale::system().language()).toInt();

    QTranslator translator;
    if (language == QLocale::German) {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("ISO-8859-15"));
        translator.load("qsm_de");
    }
    app.installTranslator(&translator);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
