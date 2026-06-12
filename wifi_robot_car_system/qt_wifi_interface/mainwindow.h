#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket> // Dung de ket noi WiFi qua giao thuc TCP

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Cac Slot xu ly su kien nut bam
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_btnForward_clicked();
    void on_btnBackward_clicked();
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnStop_clicked();
    void on_btnManualMode_clicked();
    void on_btnAutoMode_clicked();

    // Cac Slot xu ly trang thai mang
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError error);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket; // Dung QTcpSocket thay cho QBluetoothSocket

    void sendCommand(char cmd);
};

#endif // MAINWINDOW_H
