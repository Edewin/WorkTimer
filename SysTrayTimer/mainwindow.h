#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QFile>
#include <QDataStream>      // Configurations
#include <QDesktopWidget> // center the window to screen

#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include "delay.h"

#define FIRST_ELEMENT 0

#define HOURS       0
#define MINUTES     1

const quint32 HEADER_FILE = 0xE0D0A016;
#define VERSION 0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setIcon(int);

    void count();

    void on_pushButtonStart_clicked();

    void on_pushButtonSTOP_clicked();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

    void timeToGoHome();

    /*
     *  This function returns seconds for the time you need
    */
    int getTimeToCount(int hours, int minutes);

    QAction *stringAction;

    quint32 comboIndex;

    QFile *configFile;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QMenu *trayIconMenu;

    QIcon icon;
    QSystemTrayIcon *trayIcon;

    QString remainingStr;

    int remInt;

    QTime timeNow;
    QTime timeToCount;
    QTimer *counter;
    QDateTime *systemTimeDate;
};

#endif // MAINWINDOW_H
