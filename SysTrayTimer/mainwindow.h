#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QFile>
#include <QDataStream>      // Configurations
#include <QDesktopWidget> // center the window to screen
#include <QStringList>

#include <QDateTime>
#include <QDate>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

#include <QCloseEvent>

#include "binaryfileconfig.h"
#include "gohome.h"
#include "filemanipulator.h"

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

    void count();

    void on_pushButtonStart_clicked();

    void on_pushButtonSTOP_clicked();

    void on_doubleSpinBox_Hours_valueChanged(double arg1);

    void on_doubleSpinBox_Minutes_valueChanged(double arg1);

    void on_doubleSpinBox_Seconds_valueChanged(double arg1);

    void on_gifTestButton_clicked();

    void on_checkBox50MinBreak_clicked(bool checked);

    void on_comboBox_Languages_activated(int index);

    void on_comboBox_Gifs_activated(int index);

protected:
     void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

    void timeToGoHome();

    void addItemsToComboBoxes();

    // GIFs
    QStringList gifsList;


    // Counter Area

    //This function returns seconds for the time you need
    int getTimeToCount(int hrs);
    int getTimeToCount(int hrs, int mins);
    int getTimeToCount(int hrs, int mins, int secs);

    // update the value of remInt and if negative makes it positive
    void updateRemInt();

    quint8 hours, minutes, seconds;

    QAction *stringAction;

    quint32 comboIndex;

    // new cfgFile from class
    BinaryFileConfig *cfgFile;

    // csv File for logging
    FileManipulator *csvFile;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QMenu *trayIconMenu;

    QIcon icon;
    QSystemTrayIcon *trayIcon;

    QString remainingStr;

    qint32 remInt;

    QTime timeNow;
    QTime timeToCount;
    QTimer *counter;
    QDateTime *systemTimeDate;
    QDate *systemDate;
};

#endif // MAINWINDOW_H
