#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->initUI();
    isGame = false;
    timerScore = 0;
    connect(this->table, &GameTable::gameEnded, this, &MainWindow::ifPlayerWon);
}

MainWindow::~MainWindow()
{
    this->btn_start->deleteLater();
    this->timerLbl->deleteLater();

    killTimer(timerId);
}

void MainWindow::initUI()
{
    // Set window base parametres
    this->setFixedSize(QSize(400, 481));
    this->setWindowTitle("Barley Break");

    // Add menu menu bar
    dificult3 = new QAction(this);
    dificult3->setText("3x3");
    dificult4 = new QAction(this);
    dificult4->setText("4x4");
    dificult5 = new QAction(this);
    dificult5->setText("5x5");
    dificult6 = new QAction(this);
    dificult6->setText("6x6");
    dificult7 = new QAction(this);
    dificult7->setText("7x7");
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(0, 0, this->width(), 21);
    game_menu = new QMenu(this);
    game_menu->setTitle("Game");
    dificult_menu = new QMenu(this);
    dificult_menu->setTitle("dificult");
    this->setMenuBar(menuBar);

    menuBar->addAction(game_menu->menuAction());
    game_menu->addAction(dificult_menu->menuAction());
    dificult_menu->addAction(dificult3);
    dificult_menu->addAction(dificult4);
    dificult_menu->addAction(dificult5);
    dificult_menu->addAction(dificult6);
    dificult_menu->addAction(dificult7);

    connect(dificult3, &QAction::triggered, this, [this](){
        this->setDificult(3);
    });
    connect(dificult4, &QAction::triggered, this, [this](){
        this->setDificult(4);
    });
    connect(dificult5, &QAction::triggered, this, [this](){
        this->setDificult(5);
    });
    connect(dificult6, &QAction::triggered, this, [this](){
        this->setDificult(6);
    });
    connect(dificult7, &QAction::triggered, this, [this](){
        this->setDificult(7);
    });

    // Set window background
    QPixmap bkgnd("../App/Images/Textures/wood_texture.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    // Add widgets
    this->btn_start = new CustomeButton(this);
    this->btn_start->setGeometry(20, 41, 140, 40);
    this->btn_start->setText("START");
    connect(this->btn_start, &QPushButton::clicked, this, &MainWindow::toggleGame);

    this->timerLbl = new QLabel(this);
    this->timerLbl->setGeometry(180, 41, 200, 40);
    this->timerLbl->setText("00:00:00");

    QFont font;
    font.setPixelSize(28);
    font.setFamily("Bahnschrift SemiBold");

    this->timerLbl->setFont(font);
    this->timerLbl->setStyleSheet(QStringLiteral(
                                      "color: #fff;\n"
                                      ));
    this->timerLbl->setAlignment(Qt::AlignCenter);

    // Add game table
    this->table = new GameTable(this);
    table->setGeometry(20, 101, 360, 360);
    table->restoreTable(4);
    table->restoreCellSize();
    table->repaint();
}

void MainWindow::toggleGame()
{
    if (isGame == true)
    {
        isGame = false;
        this->btn_start->setText("START");
        this->table->StopGame();
        killTimer(timerId);
        timerScore = 0;
        this->timerLbl->setText("00:00:00");
    }else
    {
        isGame = true;
        this->btn_start->setText("RESTART");
        this->table->StartGame();
        timerId = startTimer(1000);
    }
}

void MainWindow::ifPlayerWon()
{
    killTimer(timerId);
    QTime time(timerScore / 3600, (timerScore / 60) % 60, timerScore % 60);

    QMessageBox msgBox;
    msgBox.setText("Congratulations, you won!\nYour total time: " + time.toString("hh:mm:ss"));
    msgBox.setStyleSheet("QLabel{min-width: 300px; font-size: 24px;}");
    msgBox.exec();

    isGame = false;
    this->btn_start->setText("START");
    this->table->StopGame();
    timerScore = 0;
    this->timerLbl->setText("00:00:00");
}

void MainWindow::setDificult(unsigned short int size)
{
    if (isGame)
        toggleGame();
    table->restoreTable(size);
    table->restoreCellSize();
    table->repaint();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    timerScore++;

    QTime time(timerScore / 3600, (timerScore / 60) % 60, timerScore % 60);

    this->timerLbl->setText(time.toString("hh:mm:ss"));
}
