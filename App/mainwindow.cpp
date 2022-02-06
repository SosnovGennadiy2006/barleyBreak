#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->initUI();
}

MainWindow::~MainWindow()
{
    this->btn_start->deleteLater();
    this->timerLbl->deleteLater();
}

void MainWindow::initUI()
{
    // Set window base parametres
    this->setFixedSize(QSize(400, 460));
    this->setWindowTitle("Barley Break");

    // Set window background
    QPixmap bkgnd("../App/Images/Textures/wood_texture.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    // Add widgets
    this->btn_start = new CustomeButton(this);
    this->btn_start->setGeometry(20, 20, 100, 40);
    this->btn_start->setText("START");

    this->timerLbl = new QLabel(this);
    this->timerLbl->setGeometry(140, 20, 240, 40);
    this->timerLbl->setText("00:00");

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
    table->setGeometry(20, 80, 360, 360);
}
