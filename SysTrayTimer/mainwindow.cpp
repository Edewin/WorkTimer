#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->setQuitLockEnabled(false); // another way to fix the bug(1) with

                                    //program crash when mainwindow is hidden

    // FramelessWindowHint
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    // get current time

    hours = ui->doubleSpinBox_Hours->value();
    minutes = ui->doubleSpinBox_Minutes->value();
    seconds = ui->doubleSpinBox_Seconds->value();

    timeNow = timeNow.currentTime();
    timeNow.start();

    remInt = getTimeToCount(hours, minutes, seconds);

    if(remInt < 0)
    {
        remInt = getTimeToCount(-hours, -minutes, -seconds);

        if(remInt < 0)
        {
            remInt = -remInt;
        }
    }



    qDebug("Remaining time is: %d s", remInt);


    systemTimeDate = new QDateTime();
    QString currentTime = systemTimeDate->currentDateTime().toString() ;
    qDebug() << currentTime;

    // counter Timer
    counter = new QTimer(this);
    counter->setInterval(1000);
    connect(counter, SIGNAL(timeout()), this, SLOT(count()) );
    counter->start();


    comboIndex = FIRST_ELEMENT;

    // add icons to comboboxes

    // Language Tab
    ui->comboBox_Languages->addItem(QIcon(":/imgs/flags/United-Kingdom.png"), tr("English"));
    ui->comboBox_Languages->addItem(QIcon(":/imgs/flags/Romania.png"), tr("Română"));
    ui->comboBox_Languages->addItem(QIcon(":/imgs/flags/Germany.png"), tr("Deutsch"));
    ui->comboBox_Languages->addItem(QIcon(":/imgs/flags/France.png"), tr("Francais"));

    // Personalize Tab
    ui->comboBox->addItem(QIcon(":/imgs/paperTimer.png"),tr("Pencil"));
    ui->comboBox->addItem(QIcon(":/imgs/hourglass.png"),tr("Glass"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock-icon.png"),tr("3D"));
    ui->comboBox->addItem(QIcon(":/imgs/TiMER-icon.png"),tr("Girl"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock02.png"),tr("B&W"));

    // configuration file
    configFile = new QFile("cfg.dat");

    configFile->open(QIODevice::ReadWrite);
    QDataStream inOut(configFile);

    // check if file containts data
    quint32 headerFile = 0;
    inOut >> headerFile;

    if(headerFile != HEADER_FILE)
    {
        qDebug() << "no data in cfg.dat";

        // write file header
        inOut << (quint32) HEADER_FILE;
        inOut << (quint32) comboIndex;

        configFile->flush();
        configFile->close();
    }

    else
    {
        // read configuration
        qDebug() << "read configurations from cfg.dat";
        inOut >> comboIndex;
        ui->comboBox->setCurrentIndex(comboIndex);

        configFile->close();
    }

    // center the MainWindow to screen
    QMainWindow::setGeometry(
                QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        window()->size(),
                        qApp->desktop()->availableGeometry()
                    )
                );

    // connect signals
    typedef void (QComboBox::*QComboIntSignal)(int);
    connect(ui->comboBox, static_cast<QComboIntSignal>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setIcon);
    connect(ui->actionHide, &QAction::triggered, this, &QWidget::hide);

    //create tray icon and show it to the world
    createActions();
    createTrayIcon();

    icon = ui->comboBox->itemIcon(comboIndex);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setIcon(int index)
{
    icon = ui->comboBox->itemIcon(index);
    trayIcon->setIcon(icon);

    setWindowIcon(icon);

    trayIcon->setToolTip(ui->comboBox->itemText(index));

    // write to cfg.dat the new index
    configFile->open(QIODevice::WriteOnly);
    QDataStream out(configFile);

    out<< (quint32) HEADER_FILE;
    out<< (quint32) index;

    configFile->flush();
    configFile->close();
}

void MainWindow::count()
{
    //qDebug()<< "time Elapsed is: " + QString::number( timeNow.elapsed() / 1000 );

    int ElapsedTime = timeNow.elapsed() / 1000;

    int buffer = remInt - ElapsedTime;

    int bufHours, bufMinutes, bufSeconds;

    if(buffer == 0)
    {
        timeToGoHome();
    }

    if(buffer < 0)
    {
        buffer = -buffer;
        ui->label_RemOrOver->setText("Overtime: ");
    }

    bufHours = buffer/ 3600;
    bufMinutes = (buffer-(bufHours*3600) ) /60;
    bufSeconds = buffer - (bufHours*3600) - (bufMinutes*60);

    remainingStr = QString::number(bufHours) + ":" + QString::number(bufMinutes) +
                   ":" + QString::number(bufSeconds);

    ui->lineEdit_remainingTime->setText( remainingStr );

    trayIcon->setToolTip( ui->label_RemOrOver->text() + remainingStr);
}

void MainWindow::createActions()
{
    stringAction = new QAction(tr("&Time Left"),this);
    connect(ui->lineEdit_remainingTime, &QLineEdit::textChanged, stringAction, &QAction::setText );

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(stringAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("time left");
}

void MainWindow::timeToGoHome()
{
    QMessageBox msgBox;
    QSize size;
    size.setHeight(200);
    size.setWidth(200);
    msgBox.setIconPixmap(trayIcon->icon().pixmap(size));
    msgBox.setText( tr("Time to go home!"));
    msgBox.exec();
}

int MainWindow::getTimeToCount(int hrs)
{
    timeToCount = timeNow.addSecs( hrs * 60 * 60 );

    return timeNow.secsTo(timeToCount);
}

int MainWindow::getTimeToCount(int hrs, int mins)
{
    timeToCount = timeNow.addSecs( hrs * 60 * 60 + mins * 60 );

    return timeNow.secsTo(timeToCount);
}

int MainWindow::getTimeToCount(int hrs, int mins, int secs)
{
    timeToCount = timeNow.addSecs( hrs * 60 * 60 + mins * 60 + secs );

    return timeNow.secsTo(timeToCount);
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(!counter->isActive())
    {
        counter->start();

        // get the time now and recalculate the time to count from new hours value
        timeNow = timeNow.currentTime();
        timeNow.restart();

        remInt = getTimeToCount(hours, minutes, seconds);

        // disable counter start button and time edit
        ui->doubleSpinBox_Hours->setEnabled(false);
        ui->doubleSpinBox_Minutes->setEnabled(false);
        ui->doubleSpinBox_Seconds->setEnabled(false);
        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonSTOP->setEnabled(true);
    }
}

void MainWindow::on_pushButtonSTOP_clicked()
{
    if(counter->isActive())
    {
        counter->stop();

        // enable counter start button and time edit
        ui->doubleSpinBox_Hours->setEnabled(true);
        ui->doubleSpinBox_Minutes->setEnabled(true);
        ui->doubleSpinBox_Seconds->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonSTOP->setEnabled(false);
    }


}

void MainWindow::on_doubleSpinBox_Hours_valueChanged(double arg1)
{
    hours = static_cast<int>(arg1);
}

void MainWindow::on_doubleSpinBox_Minutes_valueChanged(double arg1)
{
    minutes = static_cast<int>(arg1);
}

void MainWindow::on_doubleSpinBox_Seconds_valueChanged(double arg1)
{
    seconds = static_cast<int>(arg1);
}
