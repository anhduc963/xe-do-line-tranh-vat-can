/**
 * GIAI THICH CO BAN VE C++ VA QT TRONG FILE NAY:
 * 1. #include: Day la lenh 'tien xu ly', dung de 'nhap' mot thu vien ben ngoai vao code.
 *    Giong nhu ban muon dung cong cu gi thi phai lay no ra khoi hop dung cu vay.
 * 2. int main(...): Day la 'Ham Chinh', diem bat dau duy nhat cua moi chuong trinh.
 *    Khi ban chay phan mem, may tinh se tim den day dau tien de thuc hien lenh.
 * 3. argc va argv: Dung de nhan cac tham so tu dong lenh (neu co).
 *    Hien tai chung ta chi can truyen chung vao QApplication de Qt thiet lap he thong.
 */

#include "mainwindow.h" // Nhap file header cua cua so chinh do minh thiet ke
#include <QApplication> // Nhap thu vien quan ly ung dung cua Qt

int main(int argc, char *argv[])
{
    /**
     * QApplication a(argc, argv):
     * Day la viec 'tao mot doi tuong' (Object) ten la 'a' tu 'Lop' (Class) QApplication.
     * Lop giong nhu mot ban thiet ke, con Doi tuong la san pham thuc te tu ban thiet ke do.
     */
    QApplication a(argc, argv);

    /**
     * MainWindow w:
     * Tao mot doi tuong ten la 'w' tu lop 'MainWindow' (Cua so dieu khien xe).
     */
    MainWindow w;

    /**
     * w.show():
     * Goi ham 'show' cua doi tuong 'w' de hien thi cua so len man hinh.
     * Dau '.' dung de truy cap vao cac tinh nang (ham/bien) ben trong mot doi tuong.
     */
    w.show();

    /**
     * a.exec():
     * Bat dau 'Vong lap su kien' (Event Loop).
     * No giup chuong trinh 'cho doi' va 'phan hoi' khi ban bam nut hoac di chuot.
     * Neu khong co lenh nay, chuong trinh se chay vut qua va ket thuc ngay lap tuc.
     */
    return a.exec();
}
