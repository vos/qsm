#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("FH-SWF");
    app.setOrganizationDomain("fh-swf.de");
    app.setApplicationName("Qt SlideShow Manager");

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
