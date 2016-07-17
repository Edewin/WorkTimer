#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDesktopWidget> // center the window to screen

#define FIRST_ELEMENT 0

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

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayIcon();

    QAction *stringAction;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;

    QIcon icon;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
