#include "gohome.h"
#include "ui_gohome.h"

goHome::goHome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::goHome)
{
    ui->setupUi(this);

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
