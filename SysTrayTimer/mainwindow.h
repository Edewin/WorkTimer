#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QFile>
#include <QDataStream>      // Configurations
#include <QDesktopWidget> // center the window to screen
#include <QElapsedTimer>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include "delay.h"

#define FIRST_ELEMENT 0
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

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

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

    QDateTime *systemTime;
};

#endif // MAINWINDOW_H
