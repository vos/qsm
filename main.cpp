#include <QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("FH-SWF");
    app.setOrganizationDomain("fh-swf.de");
    app.setApplicationName("Qt SlideShow Manager");

    QTranslator translator;
    if (QLocale::system().language() == QLocale::German) {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("ISO-8859-15"));
        translator.load("qsm_de");
    }
    app.installTranslator(&translator);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
