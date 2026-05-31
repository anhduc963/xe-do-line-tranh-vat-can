#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // # Khoi tao agent tim kiem thiet bi
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);

    // # Khoi tao socket Bluetooth
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    // # Su dung kieu connect phu hop cho overload signal
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &MainWindow::socketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// # Slot khi tim thay thiet bi
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    ui->listDevices->addItem(device.address().toString() + " " + device.name());
}

// # Bat dau quet thiet bi
void MainWindow::on_btnScan_clicked()
{
    ui->listDevices->clear();
    discoveryAgent->start();
    ui->statusBar->showMessage("Dang quet thiet bi...");
}

// # Ket noi toi thiet bi da chon
void MainWindow::on_btnConnect_clicked()
{
    QString selected = ui->listDevices->currentItem()->text();
    QString address = selected.split(" ").at(0);

    socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::Rfcomm));
    ui->statusBar->showMessage("Dang ket noi toi " + address + "...");
}

// # Ngat ket noi
void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromService();
}

// # Xu ly khi ket noi thanh cong
void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Da ket noi Bluetooth!");
}

// # Xu ly khi ngat ket noi
void MainWindow::socketDisconnected()
{
    ui->statusBar->showMessage("Da ngat ket noi Bluetooth.");
}

// # Xu ly loi Bluetooth
void MainWindow::socketError(QBluetoothSocket::SocketError error)
{
    ui->statusBar->showMessage("Loi Bluetooth: " + QString::number(error));
}

// # Ham gui lenh
void MainWindow::sendCommand(char cmd)
{
    if (socket->isOpen()) {
        QByteArray data;
        data.append(cmd);
        socket->write(data);
        qDebug() << "Da gui lenh:" << cmd;
    } else {
        ui->statusBar->showMessage("Chua ket noi Bluetooth!");
    }
}

// # Cac ham xu ly nut bam dieu khien
void MainWindow::on_btnForward_clicked() { sendCommand('F'); }
void MainWindow::on_btnBackward_clicked() { sendCommand('B'); }
void MainWindow::on_btnLeft_clicked() { sendCommand('L'); }
void MainWindow::on_btnRight_clicked() { sendCommand('R'); }
void MainWindow::on_btnStop_clicked() { sendCommand('S'); }

// # Chon che do
void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); }
void MainWindow::on_btnLineMode_clicked() { sendCommand('A'); }
void MainWindow::on_btnObstacleMode_clicked() { sendCommand('O'); }
