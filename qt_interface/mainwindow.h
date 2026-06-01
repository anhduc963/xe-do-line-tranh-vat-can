#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // # Cac slot cho viec tim kiem va ket noi Bluetooth
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void on_btnScan_clicked();
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();

    // # Cac slot cho nut dieu khien huong
    void on_btnForward_clicked();
    void on_btnBackward_clicked();
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnStop_clicked();

    // # Cac slot cho nut chon che do
    void on_btnManualMode_clicked();
    void on_btnAutoMode_clicked();

    // # Xu ly trang thai ket noi
    void socketConnected();
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error);

private:
    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;

    // # Ham gui lenh qua Bluetooth
    void sendCommand(char cmd);
};

#endif // MAINWINDOW_H
