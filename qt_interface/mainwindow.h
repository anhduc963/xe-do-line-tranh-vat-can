#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// Thu vien ho tro tim kiem cac thiet bi Bluetooth xung quanh
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
// Thu vien ho tro ket noi va giao tiep du lieu qua Bluetooth (giong nhu Serial)
#include <QtBluetooth/QBluetoothSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * Lop MainWindow: Ke thua tu QMainWindow de tao ra cua so dieu khien robot.
 * Lop nay quan ly tat ca cac logic tu tim thiet bi, ket noi den gui lenh.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro bat buoc cua Qt de su dung co che Signal va Slot (tin hieu va ham xu ly)

public:
    // Ham khoi tao cua so
    MainWindow(QWidget *parent = nullptr);
    // Ham huy de giai phong bo nho
    ~MainWindow();

private slots:
    // Ham duoc goi khi tim thay mot thiet bi Bluetooth moi trong qua trinh quet
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

    // Cac ham xu ly khi nguoi dung nhan nut tren giao dien (Buttons)
    void on_btnScan_clicked();       // Nut "Quet Thiet Bi"
    void on_btnConnect_clicked();    // Nut "Ket Noi"
    void on_btnDisconnect_clicked(); // Nut "Ngat Ket Noi"

    // Cac ham gui lenh di chuyen khi nhan vao cac nut mui ten
    void on_btnForward_clicked();    // Tien
    void on_btnBackward_clicked();   // Lui
    void on_btnLeft_clicked();      // Re Trai
    void on_btnRight_clicked();     // Re Phai
    void on_btnStop_clicked();      // Dung xe

    // Cac ham chuyen doi che do hoat dong cua robot
    void on_btnManualMode_clicked(); // Che do Thu cong
    void on_btnAutoMode_clicked();   // Che do Tu dong

    // Cac ham lang nghe trang thai cua ket noi Bluetooth
    void socketConnected();          // Khi da ket noi thanh cong
    void socketDisconnected();       // Khi bi mat ket noi
    void socketError(QBluetoothSocket::SocketError error); // Khi co loi xay ra

private:
    // Con tro quan ly giao dien nguoi dung (duoc tao tu file .ui)
    Ui::MainWindow *ui;

    // Doi tuong dung de quet tim thiet bi Bluetooth
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

    // Doi tuong dung de duy tri ket noi va truyen nhan du lieu voi ESP32
    QBluetoothSocket *socket;

    // Ham phu tro dung de gui mot ky tu (lenh) duy nhat qua Bluetooth
    void sendCommand(char cmd);
};

#endif // MAINWINDOW_H
