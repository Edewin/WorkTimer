#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // start App with first tab index
    ui->tabWidget->setCurrentIndex(FIRST_ELEMENT);

    qApp->setQuitLockEnabled(false); // another way to fix the bug(1) with
                                    //program crash when mainwindow is hidden

    // FramelessWindowHint
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    // get current time and time to count
    hours = ui->doubleSpinBox_Hours->value();
    minutes = ui->doubleSpinBox_Minutes->value();
    seconds = ui->doubleSpinBox_Seconds->value();

    timeNow = timeNow.currentTime();
    QString startingTime = timeNow.currentTime().toString();
    timeNow.start();

    updateRemInt();

    qDebug("Remaining time is: %d s", remInt);


    systemTimeDate = new QDateTime();
    QString currentDateTime = systemTimeDate->currentDateTime().toString() ;
    qDebug() << currentDateTime;

    systemDate = new QDate();
    QString currentDate = systemDate->currentDate().toString();
    qDebug() << "current date is: " + currentDate;


    // counter Timer
    counter = new QTimer(this);
    counter->setInterval(1000);
    connect(counter, SIGNAL(timeout()), this, SLOT(count()) );
    counter->start();


    comboIndex = FIRST_ELEMENT;

    addItemsToComboBoxes();

    // configuration file
    cfgFile = new BinaryFileConfig("cfg.dat");

    // csvFile
    QString filePathString = QDir::currentPath() + "/log.csv";
    csvFile = new FileManipulator(filePathString);

   // check if file containts data
    quint32 headerFile = 0;
    headerFile = cfgFile->readHeaderFile();

    if(headerFile != HEADER_FILE)
    {
        qDebug() << "no data in cfg.dat";

        // write current date and time to log.csv
        csvFile->Append(currentDate + "," + startingTime + ",");
    }

    else
    {
        qDebug() << "read configurations from cfg.dat";

        // read configuration

        ui->checkBox50MinBreak->setChecked(cfgFile->readShortBreak());

        ui->comboBox_Languages->setCurrentIndex(cfgFile->readDesiredLanguage());
        ui->comboBox_Gifs->setCurrentIndex(cfgFile->readDesiredGif());


        QString bufDate = cfgFile->readCurrentDate();

        int bufx = QString::compare(bufDate, currentDate, Qt::CaseInsensitive);

        if(bufx == 0) // if the current date is the same
        {
            qDebug() << "same day";

            remInt = cfgFile->readRemainingTime();

            // replace last line but keep current date and starting time
            csvFile->ReplaceLastLineButKeepFirst2Elements();
        }
        else
        {
            qDebug() << "new day";

            // it`s a new day -> start over from timeToCount
            remInt = cfgFile->readTimeToCount();

            cfgFile->writeCurrentDate(currentDate);

            // write current date and time to log.csv
            csvFile->Append(currentDate + "," + startingTime + ",");
        }
        comboIndex = cfgFile->readDesiredIcon();

//        qDebug("remInt from cfg.dat is: %d", remInt);

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
    qDebug() << "destructor was called";
    remInt = remInt - (timeNow.elapsed() / 1000 );

    QString endTime = timeNow.currentTime().toString();
    int elapsedTime = timeNow.elapsed() / 1000; // seconds

    // write current date and elapsed time to log.csv
    csvFile->Append(endTime + "," + QString::number(elapsedTime) + ",\n");

    cfgFile->writeRemainingTime(remInt);

    delete csvFile;
    delete cfgFile;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "close event was called";

    event->ignore();

    QApplication::quit();
}

void MainWindow::setIcon(int index)
{
    comboIndex = index;

    icon = ui->comboBox->itemIcon(index);
    trayIcon->setIcon(icon);

    setWindowIcon(icon);

    trayIcon->setToolTip(ui->comboBox->itemText(index));

    // write to cfg.dat the new index
    cfgFile->writeDesiredIcon(comboIndex);
}

void MainWindow::count()
{
//    qDebug()<< "time Elapsed is: " + QString::number( timeNow.elapsed() / 1000 );

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

    if(ui->checkBox50MinBreak->isChecked())
    {
        if(ElapsedTime != 0 && ElapsedTime % (50 * 60) == 0)     // 50 * 60 -> 50 minutes
        {
            trayIcon->showMessage( tr("Short break!"),
                                       tr("For your healthy, you should take a short break!"));
        }
    }

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
    goHome *homeUI = new goHome(this);

    homeUI->gif = new QMovie(gifsList.at(ui->comboBox_Gifs->currentIndex()));
    homeUI->gifContainer->setMovie(homeUI->gif);
    homeUI->gif->start();
    homeUI->setOrientation(Qt::Vertical);

    if(homeUI->exec())
    {
    }

    delete homeUI;
}

void MainWindow::addItemsToComboBoxes()
{
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

        // write to cfg.dat the new timetoCount and update remInt
        cfgFile->writeTimeToCount(remInt);
        cfgFile->writeRemainingTime(remInt);

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

void MainWindow::on_gifTestButton_clicked()
{
    goHome *homeUI = new goHome(this);

    homeUI->gif = new QMovie(gifsList.at(ui->comboBox_Gifs->currentIndex()));
    homeUI->gifContainer->setMovie(homeUI->gif);
    homeUI->gif->start();
    homeUI->setOrientation(Qt::Vertical);

    if(homeUI->exec())
    {
    }

    delete homeUI;
}

void MainWindow::on_checkBox50MinBreak_clicked(bool checked)
{
    cfgFile->writeShortBreaks(checked);
    qDebug() << checked;
}

void MainWindow::on_comboBox_Languages_activated(int index)
{
    qDebug("activated index comboBox Languages is %d", index);

    cfgFile->writeDesiredLanguage(index);
}

void MainWindow::on_comboBox_Gifs_activated(int index)
{
    qDebug("index comboBox GIFs is %d", index);

    cfgFile->writeDesiredGIF(index);
}
