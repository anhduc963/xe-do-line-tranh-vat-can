#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Thiet lap phong cach giao dien (Stylesheets)
    this->setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; }"
        "QPushButton { "
        "  background-color: #ffffff; "
        "  border: 1px solid #cccccc; "
        "  border-radius: 10px; "
        "  padding: 5px; "
        "  font-weight: bold; "
        "  min-height: 30px; "
        "}"
        "QPushButton:hover { background-color: #e6e6e6; }"
        "QPushButton:pressed { background-color: #d9d9d9; }"
        "QGroupBox { "
        "  font-weight: bold; "
        "  border: 2px solid #3498db; "
        "  border-radius: 15px; "
        "  margin-top: 15px; "
        "  padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  left: 10px; "
        "  padding: 0 3px 0 3px; "
        "  color: #3498db; "
        "}"
        "#btnStop { background-color: #e74c3c; color: white; }"
        "#btnStop:hover { background-color: #c0392b; }"
        "#btnForward, #btnBackward, #btnLeft, #btnRight { font-size: 24px; color: #2c3e50; }"
        "#btnAutoMode, #btnManualMode { min-height: 50px; background-color: #2ecc71; color: white; }"
        "#btnAutoMode:hover, #btnManualMode:hover { background-color: #27ae60; }"
    );

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &MainWindow::socketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    ui->listDevices->addItem(device.address().toString() + " " + device.name());
}

void MainWindow::on_btnScan_clicked()
{
    ui->listDevices->clear();
    discoveryAgent->start();
    ui->statusBar->showMessage("Đang quét thiết bị...");
}

void MainWindow::on_btnConnect_clicked()
{
    if (!ui->listDevices->currentItem()) return;
    QString selected = ui->listDevices->currentItem()->text();
    QString address = selected.split(" ").at(0);

    socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::ProtocolUuid::Rfcomm));
    ui->statusBar->showMessage("Đang kết nối tới " + address + "...");
}

void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromService();
}

void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Đã kết nối Bluetooth!");
}

void MainWindow::socketDisconnected()
{
    ui->statusBar->showMessage("Đã ngắt kết nối Bluetooth.");
}

void MainWindow::socketError(QBluetoothSocket::SocketError error)
{
    ui->statusBar->showMessage("Lỗi Bluetooth: " + socket->errorString());
}

void MainWindow::sendCommand(char cmd)
{
    // Ky tu dieu khien: F: Tien, B: Lui, L: Trai, R: Phai, S: Dung, M: Thu cong, A: Tu dong
    if (socket->isOpen()) {
        QByteArray data;
        data.append(cmd);
        socket->write(data);
        qDebug() << "Da gui lenh:" << cmd;
    } else {
        ui->statusBar->showMessage("Chưa kết nối Bluetooth!");
    }
}

void MainWindow::on_btnForward_clicked() { sendCommand('F'); }
void MainWindow::on_btnBackward_clicked() { sendCommand('B'); }
void MainWindow::on_btnLeft_clicked() { sendCommand('L'); }
void MainWindow::on_btnRight_clicked() { sendCommand('R'); }
void MainWindow::on_btnStop_clicked() { sendCommand('S'); }

void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); }
void MainWindow::on_btnAutoMode_clicked() { sendCommand('A'); }
