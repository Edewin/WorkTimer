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

#include <QTranslator>

#define FIRST_ELEMENT 0

const quint32 HEADER_FILE = 0xE0D0A016;
#define VERSION 0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_ENUMS(Languages)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();  


    enum class Languages {English, Romanian, German, French };

private slots:
    void setIcon(int);

    void count();

    void on_pushButtonStart_clicked();

    void on_pushButtonSTOP_clicked();

    void on_doubleSpinBox_Hours_valueChanged(double arg1);

    void on_doubleSpinBox_Minutes_valueChanged(double arg1);

    void on_doubleSpinBox_Seconds_valueChanged(double arg1);

    void on_comboBox_Languages_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

    void timeToGoHome();


    // Counter Area

    //This function returns seconds for the time you need
    int getTimeToCount(int hrs);
    int getTimeToCount(int hrs, int mins);
    int getTimeToCount(int hrs, int mins, int secs);

    int hours, minutes, seconds;


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
