#include "mainwindow.h"

#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::srand(std::time(nullptr));

    MainWindow w;
    w.show();

    return a.exec();
}
