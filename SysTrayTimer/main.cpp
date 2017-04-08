#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

   // a.setQuitOnLastWindowClosed(false); // bug(1) fix

//    w.show();

    return a.exec();
}
