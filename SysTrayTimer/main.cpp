#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTranslator translator;

    translator.load("test");

    a.installTranslator(&translator);
   // a.setQuitOnLastWindowClosed(false); // bug(1) fix

    w.show();

    return a.exec();
}
