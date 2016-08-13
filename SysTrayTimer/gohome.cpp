#include "gohome.h"
#include "ui_gohome.h"

goHome::goHome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goHome)
{
    ui->setupUi(this);

    this->move(-200, -200);
    // center the QDialog to screen
    QDialog::setGeometry(
                QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignAbsolute | Qt::AlignCenter,
                        window()->size(),
                        qApp->desktop()->availableGeometry()
                    )
                );

//    gif = new QMovie(this);
    gifContainer = new QLabel(this);

    layout = new QVBoxLayout;
    layout->addWidget(gifContainer);


    this->setLayout(layout);
    this->show();

}

goHome::~goHome()
{
    delete ui;
}
