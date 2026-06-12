#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    // Khoi tao socket mang
    socket = new QTcpSocket(this);

    // Ket noi cac tin hieu trang thai
    connect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::socketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    QString ip = ui->editIP->text();
    int port = ui->editPort->text().toInt();

    socket->connectToHost(ip, port);
    ui->statusBar->showMessage("Dang ket noi toi " + ip + ":" + QString::number(port) + "...");
}

void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromHost();
}

void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Da ket noi WiFi (TCP)!");
}

void MainWindow::socketDisconnected()
{
    ui->statusBar->showMessage("Da ngat ket noi WiFi.");
}

void MainWindow::socketError(QAbstractSocket::SocketError error)
{
    ui->statusBar->showMessage("Loi WiFi: " + QString::number((int)error));
}

void MainWindow::sendCommand(char cmd)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        data.append(cmd);
        socket->write(data);
        socket->flush(); // Dam bao du lieu duoc gui di ngay lap tuc
        qDebug() << "Da gui lenh WiFi:" << cmd;
    } else {
        ui->statusBar->showMessage("Chua ket noi WiFi!");
    }
}

void MainWindow::on_btnForward_clicked() { sendCommand('F'); }
void MainWindow::on_btnBackward_clicked() { sendCommand('B'); }
void MainWindow::on_btnLeft_clicked() { sendCommand('L'); }
void MainWindow::on_btnRight_clicked() { sendCommand('R'); }
void MainWindow::on_btnStop_clicked() { sendCommand('S'); }
void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); }
void MainWindow::on_btnAutoMode_clicked() { sendCommand('A'); }
