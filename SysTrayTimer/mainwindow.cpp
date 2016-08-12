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

    // get current time and time to count

    hours = ui->doubleSpinBox_Hours->value();
    minutes = ui->doubleSpinBox_Minutes->value();
    seconds = ui->doubleSpinBox_Seconds->value();

    timeNow = timeNow.currentTime();
    timeNow.start();

    updateRemInt();

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

    // Add icons
    ui->comboBox->addItem(QIcon(":/imgs/paperTimer.png"),tr("Pencil"));
    ui->comboBox->addItem(QIcon(":/imgs/hourglass.png"),tr("Glass"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock-icon.png"),tr("3D"));
    ui->comboBox->addItem(QIcon(":/imgs/TiMER-icon.png"),tr("Girl"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock02.png"),tr("B&W"));

    // Add GIFs

    gifsList << ":/imgs/gifs/clear.gif" << ":/imgs/gifs/cry.gif"
             << ":/imgs/gifs/Foam-Heart.gif" << ":/imgs/gifs/friendfsgif"
             << ":/imgs/gifs/giphy.gif" <<  ":/imgs/gifs/giphyElectric.gif"
             << ":/imgs/gifs/go.gif" << ":/imgs/gifs/gohme.gif"
             << ":/imgs/gifs/Golf-Ball.gif" << ":/imgs/gifs/sleepy.gif"
             << ":/imgs/gifs/tumblr_inline.gif" << ":/imgs/gifs/wGDqq.gif";

    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/clear.gif"), tr("clear"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/cry.gif"), tr("cry"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/Foam-Heart.gif"), tr("heart coffee"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/friendfsgif"), tr("friends fan"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/giphy.gif"), tr("giphy"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/giphyElectric.gif"), tr("electric"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/go.gif"), tr("just go"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/gohme.gif"), tr("go home"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/Golf-Ball.gif"), tr("golf"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/sleepy.gif"), tr("sleepy"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/tumblr_inline.gif"), tr("woman"));
    ui->comboBox_Gifs->addItem(QIcon(":/imgs/gifs/wGDqq.gif"), tr("work done"));

    // configuration file
    configFile = new QFile("cfg.dat");

    configFile->open(QIODevice::ReadWrite);
    QDataStream inOut(configFile);

    // check if file containts data
    quint32 headerFile = 0;
    inOut >> headerFile;

    if(headerFile != HEADER_FILE)
    {
        //qDebug() << "no data in cfg.dat";

        // write file header
        inOut << (quint32) HEADER_FILE;
        inOut << (quint32) comboIndex;
        inOut << (qint32) remInt;

        configFile->flush();
        configFile->close();
    }

    else
    {
        //qDebug() << "read configurations from cfg.dat";

        // read configuration
        inOut >> comboIndex;
        inOut >> remInt;

        qDebug("remInt from cfg.dat is: %d", remInt);

        ui->comboBox->setCurrentIndex(comboIndex);

        // update doubleSpinBoxes with the value read from file
        int bufHours, bufMinutes, bufSeconds;

        if(remInt < 0)
        {
            ui->label_RemOrOver->setText( tr("Overtime: ") );
        }

        bufHours = remInt/ 3600;
        bufMinutes = (remInt-(bufHours*3600) ) /60;
        bufSeconds = remInt - (bufHours*3600) - (bufMinutes*60);

        ui->doubleSpinBox_Hours->setValue(bufHours);
        ui->doubleSpinBox_Minutes->setValue(bufMinutes);
        ui->doubleSpinBox_Seconds->setValue(bufSeconds);

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
    remInt = remInt - (timeNow.elapsed() /1000 );

    writeToCfgFile(comboIndex, remInt);

    delete ui;
}


void MainWindow::setIcon(int index)
{
    comboIndex = index;

    icon = ui->comboBox->itemIcon(index);
    trayIcon->setIcon(icon);

    setWindowIcon(icon);

    trayIcon->setToolTip(ui->comboBox->itemText(index));

    // write to cfg.dat the new index
    writeToCfgFile(index, remInt);
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
    else
    {
        ui->label_RemOrOver->setText("Remaining Time: ");
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

void MainWindow::writeToCfgFile(quint32 iconIndex, qint32 remainingTime)
{
    configFile->open(QIODevice::WriteOnly);
    QDataStream out(configFile);

    out<< (quint32) HEADER_FILE;
    out<< (quint32) iconIndex;
    out<< (qint32) remainingTime;

    configFile->flush();
    configFile->close();
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

void MainWindow::updateRemInt()
{
    remInt = getTimeToCount(hours, minutes, seconds);

    if(remInt < 0)
    {
        remInt = getTimeToCount(-hours, -minutes, -seconds);

        if(remInt < 0)
        {
            remInt = -remInt;
        }
    }

}

void MainWindow::on_pushButtonStart_clicked()
{
    if(!counter->isActive())
    {
        counter->start();

        // get the time now and recalculate the time to count from new hours value
        timeNow = timeNow.currentTime();
        timeNow.restart();

        updateRemInt();

        writeToCfgFile( comboIndex, remInt );

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

void MainWindow::on_comboBox_Languages_currentIndexChanged(int index)
{
    // ToDo add language in future
}

void MainWindow::on_comboBox_Gifs_currentIndexChanged(int index)
{
//    qDebug("index from gifs changed");

}

void MainWindow::on_gifTestButton_clicked()
{
    goHome *homeUI = new goHome(this);

    homeUI->gif = new QMovie(gifsList.at(ui->comboBox_Gifs->currentIndex()));
    homeUI->gifContainer->setMovie(homeUI->gif);
    homeUI->gif->start();


    if(homeUI->exec())
    {
    }

    delete homeUI;
}
