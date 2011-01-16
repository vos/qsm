#include <QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if (QLocale::system().language() == QLocale::German) {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("ISO-8859-15"));
        translator.load("qsm_de");
    }
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
