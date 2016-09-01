#include "graphs_last_week.h"
#include "ui_graphs_last_week.h"

Graphs_last_week::Graphs_last_week(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Graphs_last_week)
{
    ui->setupUi(this);
}

Graphs_last_week::~Graphs_last_week()
{
    delete ui;
}
