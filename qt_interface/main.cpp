#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Khoi tao ung dung Qt
    QApplication a(argc, argv);

    // Tao va hien thi cua so chinh
    MainWindow w;
    w.show();

    // Chay vong lap su kien
    return a.exec();
}
