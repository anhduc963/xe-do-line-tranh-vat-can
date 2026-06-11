#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/**
 * GIAI THICH VE TOAN TU PHAM VI '::' (Scope Resolution Operator):
 * 'MainWindow::MainWindow' nghia la: "Day la dinh nghia cho ham MainWindow thuoc ve Lop MainWindow".
 * No giup may tinh biet ham nay nam o dau, tranh nham lan voi cac ham cung ten o lop khac.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) // Goi ham khoi tao cua Lop Cha (QMainWindow) de thiet lap cua so
    , ui(new Ui::MainWindow) // Tao moi doi tuong giao dien
{
    /**
     * GIAI THICH VE 'this->':
     * 'this' la mot con tro dac biet, no tro den CHINH doi tuong hien tai ma chung ta dang viet code.
     * 'this->ui' nghia la: "Lay bien 'ui' nam ben trong chinh cua so nay".
     * No giup lam ro rang chung ta dang thao tac voi thanh phan nao cua Lop.
     */
    ui->setupUi(this); // Thiet lap giao dien len chinh cua so nay

    // Dung Stylesheet de trang tri giao dien (Giong nhu CSS trong Web)
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
        "#btnStop { background-color: #e74c3c; color: white; }"
    );

    /**
     * NEW TRONG C++:
     * 'new' dung de 'Cap phat bo nho dong'. No tao ra mot doi tuong moi va tra ve mot con tro.
     * 'discoveryAgent = new ...' nghia la: "Tao bo quet moi va luu dia chi cua no vao bien discoveryAgent".
     */
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    /**
     * GIAI THICH VE CONNECT (KET NOI TIN HIEU):
     * connect(Nguon_phat, Tin_hieu, Doi_tuong_nhan, Ham_xu_ly);
     * No giong nhu viec ban cam day dien: Khi 'Nguon_phat' phat ra 'Tin_hieu',
     * thi 'Ham_xu_ly' ben trong 'Doi_tuong_nhan' se chay.
     */
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &MainWindow::deviceDiscovered);

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &MainWindow::socketError);
}

MainWindow::~MainWindow()
{
    /**
     * delete: Dung de giai phong vung nho da cap phat bang 'new'.
     * Neu khong delete, may tinh se bi day bo nho (Memory Leak).
     */
    delete ui;
}

void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    /**
     * '->' (Dau mui ten):
     * Dung de truy cap vao ham/bien cua mot doi tuong thong qua CON TRO.
     * 'ui->listDevices' nghia la: "Vao doi tuong giao dien (ui), tim thanh phan danh sach (listDevices)".
     */
    ui->listDevices->addItem(device.address().toString() + " " + device.name());
}

void MainWindow::on_btnScan_clicked()
{
    ui->listDevices->clear();
    discoveryAgent->start();
    ui->statusBar->showMessage("Dang quet thiet bi...");
}

void MainWindow::on_btnConnect_clicked()
{
    if (!ui->listDevices->currentItem()) return;

    QString selected = ui->listDevices->currentItem()->text();
    // split(" "): Chia chuoi ky tu ra lam nhieu phan dua tren dau cach
    QString address = selected.split(" ").at(0);

    socket->connectToService(QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::Rfcomm));
    ui->statusBar->showMessage("Dang ket noi toi " + address + "...");
}

void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromService();
}

void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Da ket noi Bluetooth!");
}

void MainWindow::socketDisconnected()
{
    ui->statusBar->showMessage("Da ngat ket noi Bluetooth.");
}

void MainWindow::socketError(QBluetoothSocket::SocketError error)
{
    // QString::number(): Chuyen doi kieu so sang kieu chu de hien thi
    ui->statusBar->showMessage("Loi Bluetooth: " + QString::number(error));
}

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

// Cac ham duoi day gui ky tu lenh duy nhat ('F', 'B', 'L', 'R', 'S', 'M', 'A')
// ESP32 se doc cac ky tu nay va dieu khien cac chan IN1-IN4 tuong ung.
void MainWindow::on_btnForward_clicked() { sendCommand('F'); }
void MainWindow::on_btnBackward_clicked() { sendCommand('B'); }
void MainWindow::on_btnLeft_clicked() { sendCommand('L'); }
void MainWindow::on_btnRight_clicked() { sendCommand('R'); }
void MainWindow::on_btnStop_clicked() { sendCommand('S'); }
void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); }
void MainWindow::on_btnAutoMode_clicked() { sendCommand('A'); }
