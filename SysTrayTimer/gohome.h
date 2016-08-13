#ifndef GOHOME_H
#define GOHOME_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget> // center the window to screen

#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>


namespace Ui {
class goHome;
}

class goHome : public QDialog
{
    Q_OBJECT

public:
    explicit goHome(QWidget *parent = 0);
    ~goHome();

    QLabel *gifContainer;
    QMovie *gif;
    QVBoxLayout *layout;

private:
    Ui::goHome *ui;
};

#endif // GOHOME_H
