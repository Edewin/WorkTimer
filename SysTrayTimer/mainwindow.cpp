#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // center the MainWindow to screen
    QMainWindow::setGeometry(
                QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        window()->size(),
                        qApp->desktop()->availableGeometry()
                    )
                );


    // add icons to combobox
    ui->comboBox->addItem(QIcon(":/imgs/paperTimer.png"),tr("Pencil"));
    ui->comboBox->addItem(QIcon(":/imgs/hourglass.png"),tr("Glass"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock-icon.png"),tr("3D"));
    ui->comboBox->addItem(QIcon(":imgs/TiMER-icon.png"),tr("Girl"));
    ui->comboBox->addItem(QIcon(":/imgs/Clock02.png"),tr("B&W"));

    // connect signals
    typedef void (QComboBox::*QComboIntSignal)(int);
    connect(ui->comboBox, static_cast<QComboIntSignal>(&QComboBox::currentIndexChanged),
            this, &MainWindow::setIcon);


    //create tray icon and show it to the world
    createActions();
    createTrayIcon();

    icon = ui->comboBox->itemIcon(FIRST_ELEMENT);
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
