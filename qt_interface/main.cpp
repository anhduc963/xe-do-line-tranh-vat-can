#include "mainwindow.h"
#include <QApplication>

/**
 * Ham main: Diem bat dau cua moi chuong trinh C++
 * argc: So luong tham so dong lenh
 * argv: Mang cac tham so dong lenh
 */
int main(int argc, char *argv[])
{
    // Khoi tao doi tuong QApplication de quan ly vong lap su kien va cac thiet lap he thong cho Qt
    QApplication a(argc, argv);

    // Tao mot doi tuong thuoc lop MainWindow (cua so chinh chung ta da thiet ke)
    MainWindow w;

    // Hien thi cua so len man hinh
    w.show();

    // Bat dau vong lap su kien (event loop). Chuong trinh se tiep tuc chay cho den khi cua so bi dong.
    return a.exec();
}
