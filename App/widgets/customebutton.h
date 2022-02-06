#ifndef CUSTOMEBUTTON_H
#define CUSTOMEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QCursor>
#include <QFont>
#include <QPushButton>

class CustomeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomeButton(QWidget *parent = nullptr);

    void initUI();

signals:

};

#endif // CUSTOMEBUTTON_H
