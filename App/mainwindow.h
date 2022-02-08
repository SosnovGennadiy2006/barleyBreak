#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPalette>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimerEvent>
#include <QTime>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <widgets/customebutton.h>
#include <widgets/gametable.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QPushButton* btn_start;
    QLabel* timerLbl;
    GameTable* table;
    int timerId;
    int timerScore;
    bool isGame;

    QAction* dificult3;
    QAction* dificult4;
    QAction* dificult5;
    QAction* dificult6;
    QAction* dificult7;
    QMenuBar* menuBar;
    QMenu* game_menu;
    QMenu* dificult_menu;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUI();
    void toggleGame();
    void ifPlayerWon();
    void setDificult(unsigned short int size);

signals:

protected:
    void timerEvent(QTimerEvent* event);
};

#endif // MAINWINDOW_H
