#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPalette>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <widgets/customebutton.h>
#include <widgets/gametable.h>

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QPushButton* btn_start;
    QLabel* timerLbl;
    GameTable* table;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUI();

signals:

};

#endif // MAINWINDOW_H
