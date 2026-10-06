#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //chinh sua giao dien
    this->setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; color: #333333; }"
        "QGroupBox { color: #333333; font-weight: bold; border: 1px solid #cccccc; border-radius: 8px;}"
        "QLabel { color: #333333; }"

        "QLineEdit { "
        "  background-color: #ffffff; "
        "  color: #000000; "
        "  border: 1px solid #cccccc; "
        "  border-radius: 6px; "
        "  padding: 4px; "
        "}"

        "QPushButton { "
        "  background-color: #ffffff; "
        "  color: #333333; "
        "  border: 1px solid #cccccc; "
        "  border-radius: 10px; "
        "  padding: 5px; "
        "  font-weight: bold; "
        "  min-height: 10px; "
        "}"
        "QPushButton:hover {"
        "   background-color: #d5d5d5;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #b0b0b0;"
        "   border: 1px solid #707070;"
        "}"

        "#btnForward, #btnBackward, #btnLeft, #btnRight, #btnStop { "
        "  font-size: 18px; "
        "  min-width: 40px; "
        "  min-height: 40px; "
        "}"

        "#btnStop { "
        "  background-color: #e74c3c; "
        "  color: white; "
        "  border: 1px solid #c0392b; "
        "}"
        "#btnStop:hover { "
        "  background-color: #ff6b5b; "
        "}"
        "#btnStop:pressed { "
        "  background-color: #c0392b; "
        "  border: 1px solid #962d22; "
        "}"
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
    ui->statusBar->setStyleSheet("color: black;");
    ui->statusBar->showMessage("Dang ket noi...");
}

void MainWindow::on_btnDisconnect_clicked()
{
    socket->disconnectFromHost();
}

void MainWindow::socketConnected()
{
    ui->statusBar->showMessage("Da ket noi WiFi!");
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
        socket->flush();
        qDebug() << "Da gui lenh WiFi:" << cmd;
    } else {
        ui->statusBar->showMessage("Chua ket noi WiFi!");
    }
}

void MainWindow::on_btnForward_clicked()    { sendCommand('F'); }
void MainWindow::on_btnBackward_clicked()   { sendCommand('B'); }
void MainWindow::on_btnLeft_clicked()       { sendCommand('L'); }
void MainWindow::on_btnRight_clicked()      { sendCommand('R'); }
void MainWindow::on_btnStop_clicked()       { sendCommand('S');}
void MainWindow::on_btnManualMode_clicked() { sendCommand('M'); }
void MainWindow::on_btnAutoMode_clicked()   { sendCommand('A'); }
