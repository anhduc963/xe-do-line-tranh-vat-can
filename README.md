# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH QUA GIAO DIỆN QT

Hệ thống này cho phép bạn điều khiển xe tự hành bằng máy tính hoặc điện thoại thông qua Bluetooth, với 2 chế độ: **Thủ công** (Manual) và **Tự động** (Auto - Dò line kết hợp tránh vật cản).

## 1. Chuẩn bị phần cứng và Đấu nối

### Các linh kiện chính:
- **Arduino Uno R3**: Bộ điều khiển trung tâm.
- **ESP32**: Module Bluetooth.
- **L298N**: Mạch công suất động cơ.
- **HC-SR04**: Cảm biến siêu âm.
- **TCRT5000**: 4 mắt cảm biến dò line.

### Sơ đồ đấu nối quan trọng:
1.  **Arduino <-> ESP32 (Giao tiếp Serial)**:
    - Arduino RX (0) <--> ESP32 TX (17)
    - Arduino TX (1) <--> ESP32 RX (16)
    - **Lưu ý**: Phải nối chung chân GND giữa Arduino và ESP32.
2.  **Arduino <-> L298N (Động cơ)**:
    - ENA: 5, IN1: 7, IN2: 8, IN3: 9, IN4: 11, ENB: 6
3.  **Cảm biến**:
    - Siêu âm: Trig (12), Echo (13)
    - Dò line: A0, A1, A2, A3
    - Servo: 10

## 2. Nạp Code cho phần cứng

1.  **Nạp cho Arduino**:
    - Mở file `arduino_car/arduino_car.ino` bằng Arduino IDE.
    - Chọn Board là "Arduino Uno".
    - **Quan trọng**: Rút dây nối RX/TX với ESP32 trước khi nạp.
    - Nhấn Upload.
2.  **Nạp cho ESP32**:
    - Mở file `esp32_bluetooth_bridge/esp32_bluetooth_bridge.ino`.
    - Chọn Board là "ESP32 Dev Module".
    - Nhấn Upload.

## 3. Cách sử dụng Giao diện Qt trên máy tính

### Cài đặt môi trường:
- Bạn cần cài đặt **Qt Creator** và bộ công cụ **Qt 5.15 hoặc Qt 6** có bao gồm module **Bluetooth**.

### Chạy ứng dụng:
1.  Mở file `qt_interface/RobotControl.pro` trong Qt Creator.
2.  Nhấn nút **Run** (hình tam giác xanh) để biên dịch và chạy giao diện.

### Các bước điều khiển:
1.  **Bật Bluetooth**: Đảm bảo Bluetooth trên máy tính đã được bật.
2.  **Quét thiết bị**: Nhấn nút **"Quét Thiết Bị"**.
3.  **Kết nối**: Tìm và chọn thiết bị có tên `RobotCar_Bluetooth` trong danh sách, sau đó nhấn **"Kết Nối"**.
4.  **Điều khiển**:
    - Chọn **"Chế Độ Thủ Công"** để tự lái bằng các nút mũi tên (↑, ↓, ←, →) và nút Dừng (■).
    - Chọn **"Chế Độ Tự Động"** để xe tự dò line và tự tránh vật cản.

---
*Lưu ý: Nếu dùng điện thoại, bạn cũng có thể cài các app "Serial Bluetooth Terminal" để gửi các ký tự: 'F', 'B', 'L', 'R', 'S', 'M', 'A' để điều khiển tương ứng.*
