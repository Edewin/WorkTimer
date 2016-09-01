#ifndef GRAPHS_LAST_WEEK_H
#define GRAPHS_LAST_WEEK_H

#include <QMainWindow>

namespace Ui {
class Graphs_last_week;
}

class Graphs_last_week : public QMainWindow
{
    Q_OBJECT

public:
    explicit Graphs_last_week(QWidget *parent = 0);
    ~Graphs_last_week();

private:
    Ui::Graphs_last_week *ui;
};

#endif // GRAPHS_LAST_WEEK_H
