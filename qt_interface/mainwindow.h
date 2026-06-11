#ifndef MAINWINDOW_H // Lenh chong 'dinh nghia chong cheo' (neu file duoc include nhieu noi)
#define MAINWINDOW_H

#include <QMainWindow> // Cua so chinh cua Qt
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent> // Thu vien quet Bluetooth
#include <QtBluetooth/QBluetoothSocket> // Thu vien truyen nhan du lieu Bluetooth

/**
 * GIAI THICH VE LOP (CLASS) VA KE THUA:
 * 1. class MainWindow : public QMainWindow
 *    - 'class': Tu khoa de khai bao mot Lop (Ban thiet ke).
 *    - ':': Dau hai cham nay bieu thi su 'Ke thua'.
 *    - 'MainWindow' se thua huong tat ca tinh nang cua 'QMainWindow' (nhu thanh tieu de, menu, v.v.)
 *      va chung ta chi viec viet them cac tinh nang rieng cho robot.
 *
 * 2. Q_OBJECT: Day la mot Macro dac biet cua Qt.
 *    No giup C++ (von la ngon ngu cu) hieu duoc cac tinh nang hien dai cua Qt nhu Signal va Slot.
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // 'namespace' giup phan biet ten Lop giua code cua minh va code tu dong cua UI
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * public: Cac ham o day co the duoc goi tu bat cu dau ben ngoai Lop.
     * explicit MainWindow(QWidget *parent = nullptr):
     * Day la 'Ham khoi tao' (Constructor). No chay ngay khi doi tuong MainWindow duoc tao ra.
     * '*' (Dau sao): Ky hieu cua 'Con tro' (Pointer). No luu dia chi vung nho cua doi tuong.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    // Ham huy (Destructor): Chay khi cua so bi dong de xoa rac trong bo nho.
    ~MainWindow();

private slots:
    /**
     * slots: Day la cac 'O cam'. Khi co mot 'Tin hieu' (Signal) - nhu bam nut - truyen den,
     * thi ham ben trong Slot se duoc kich hoat de xu ly.
     */
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void on_btnScan_clicked();
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_btnForward_clicked();
    void on_btnBackward_clicked();
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnStop_clicked();
    void on_btnManualMode_clicked();
    void on_btnAutoMode_clicked();
    void socketConnected();
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error);

private:
    /**
     * private: Cac bien va ham o day chi co the duoc dung ben trong noi bo Lop MainWindow.
     * Ben ngoai khong the 'nhin thay' hoac thay doi chung truc tiep.
     */
    Ui::MainWindow *ui; // Con tro den giao dien (duoc thiet ke trong file .ui)
    QBluetoothDeviceDiscoveryAgent *discoveryAgent; // Con tro den bo quet Bluetooth
    QBluetoothSocket *socket; // Con tro den bo truyen nhan du lieu

    void sendCommand(char cmd); // Ham gui lenh noi bo
};

#endif // MAINWINDOW_H
