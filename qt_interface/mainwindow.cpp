#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/**
 * Ham khoi tao: Thiet lap ban dau cho cua so chinh
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Khoi tao cac thanh phan giao dien tu file .ui
    ui->setupUi(this);

    // Thiet lap phong cach (CSS) cho giao dien de co giao dien hien dai hon
    // Dung Stylesheet de bo tron goc nut (border-radius) va dat mau sac
    this->setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; }" // Mau nen cua so
        "QPushButton { "
        "  background-color: #ffffff; "
        "  border: 1px solid #cccccc; "
        "  border-radius: 10px; " // Bo tron goc 10px
        "  padding: 5px; "
        "  font-weight: bold; "
        "  min-height: 30px; "
        "}"
        "QPushButton:hover { background-color: #e6e6e6; }" // Mau khi di chuot qua
        "QGroupBox { "
        "  font-weight: bold; "
        "  border: 2px solid #3498db; " // Khung nhom xanh duong
        "  border-radius: 15px; "
        "  margin-top: 15px; "
        "}"
        "#btnStop { background-color: #e74c3c; color: white; }" // Nut dung mau do
        "#btnAutoMode, #btnManualMode { min-height: 50px; background-color: #2ecc71; color: white; }" // Nut che do mau xanh la
    );

    // Tao doi tuong quet thiet bi Bluetooth
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // Ket noi tin hieu 'tim thay thiet bi' voi ham xu ly 'deviceDiscovered'
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);

    // Tao doi tuong Socket de giao tiep theo giao thuc RFCOMM (giao tiep Serial qua Bluetooth)
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    // Ket noi cac tin hieu trang thai cua socket voi cac ham tuong ung
    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &MainWindow::socketError);
}

/**
 * Ham huy: Don dep bo nho khi cua so bi dong
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Ham xu ly khi tim thay mot thiet bi Bluetooth
 * device: Thong tin thiet bi tim thay
 */
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // Them dia chi MAC va ten thiet bi vao danh sach hien thi tren giao dien
    ui->listDevices->addItem(device.address().toString() + " " + device.name());
}

/**
 * Ham xu ly khi nhan nut "Quet Thiet Bi"
 */
void MainWindow::on_btnScan_clicked()
{
    ui->listDevices->clear(); // Xoa danh sach cu
    discoveryAgent->start();  // Bat dau qua trinh quet
    ui->statusBar->showMessage("Dang quet thiet bi..."); // Hien thong bao o thanh trang thai
}

/**
 * Ham xu ly khi nhan nut "Ket Noi"
 */
void MainWindow::on_btnConnect_clicked()
{
    // Kiem tra xem nguoi dung da chon thiet bi nao trong danh sach chua
    if (!ui->listDevices->currentItem()) return;

    // Lay chuoi ky tu duoc chon (VD: "AA:BB:CC:DD:EE:FF MyRobot")
    QString selected = ui->listDevices->currentItem()->text();
    // Tach lay dia chi MAC (phan dau tien truoc dau cach)
    QString address = selected.split(" ").at(0);

    // Thuc hien ket noi den dia chi MAC do qua dich vu RFCOMM (Serial)
    socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::Rfcomm));
    ui->statusBar->showMessage("Dang ket noi toi " + address + "...");
}

/**
 * Ham xu ly khi nhan nut "Ngat Ket Noi"
 */
void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromService(); // Ngat ket noi hien tai
}

/**
 * Ham duoc goi tu dong khi ket noi Bluetooth thanh cong
 */
void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Da ket noi Bluetooth!");
}

/**
 * Ham duoc goi tu dong khi bi ngat ket noi
 */
void MainWindow::socketDisconnected()
{
    ui->statusBar->showMessage("Da ngat ket noi Bluetooth.");
}

/**
 * Ham xu ly khi xay ra loi trong qua trinh ket noi
 */
void MainWindow::socketError(QBluetoothSocket::SocketError error)
{
    ui->statusBar->showMessage("Loi Bluetooth: " + QString::number(error));
}

/**
 * Ham gui lenh den ESP32
 * cmd: Ky tu lenh (VD: 'F' la di thang)
 */
void MainWindow::sendCommand(char cmd)
{
    // Kiem tra xem ket noi dang mo hay khong
    if (socket->isOpen()) {
        QByteArray data; // Tao mang byte de gui du lieu
        data.append(cmd); // Them ky tu lenh vao mang
        socket->write(data); // Gui du lieu qua Bluetooth
        qDebug() << "Da gui lenh:" << cmd; // Ghi log de kiem tra
    } else {
        // Neu chua ket noi thi bao loi cho nguoi dung
        ui->statusBar->showMessage("Chua ket noi Bluetooth!");
    }
}

// Cac ham Slot ket noi truc tiep tu giao dien UI (thiet ke trong mainwindow.ui)
void MainWindow::on_btnForward_clicked() { sendCommand('F'); }   // Nut Tien -> Gui 'F'
void MainWindow::on_btnBackward_clicked() { sendCommand('B'); }  // Nut Lui -> Gui 'B'
void MainWindow::on_btnLeft_clicked() { sendCommand('L'); }      // Nut Trai -> Gui 'L'
void MainWindow::on_btnRight_clicked() { sendCommand('R'); }     // Nut Phai -> Gui 'R'
void MainWindow::on_btnStop_clicked() { sendCommand('S'); }      // Nut Dung -> Gui 'S'

void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); } // Chon che do Thu cong -> Gui 'M'
void MainWindow::on_btnAutoMode_clicked() { sendCommand('A'); }   // Chon che do Tu dong -> Gui 'A'
