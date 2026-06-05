# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH (DÙNG DUY NHẤT ESP32)

Hệ thống này đã được tối ưu để chạy hoàn toàn trên **một mạch ESP32 duy nhất**, thay thế cho bộ Arduino + ESP32 trước đây.

## 1. Chuẩn bị phần cứng và Đấu nối (ESP32)

### Các linh kiện chính:
- **ESP32 Dev Module**: Bộ điều khiển chính và Bluetooth.
- **L298N**: Mạch công suất động cơ DC.
- **HC-SR04**: Cảm biến siêu âm.
- **TCRT5000**: 4 mắt cảm biến dò line.
- **SG90**: Động cơ servo.

### Sơ đồ đấu nối GPIO trên ESP32:
1.  **Động cơ (L298N)**:
    - ENA: 32, IN1: 33, IN2: 25, IN3: 26, IN4: 27, ENB: 14
2.  **Cảm biến siêu âm**:
    - Trig: 12, Echo: 13
3.  **Cảm biến dò line**:
    - LO: 34, LI: 35, RI: 36, RO: 39 (Các chân Input Only trên ESP32)
4.  **Servo**: 15

## 2. Nạp Code cho ESP32

1.  **Cài đặt thư viện**:
    - Cần cài đặt thư viện **ESP32Servo** trong Arduino IDE (Tools -> Manage Libraries).
2.  **Nạp Code**:
    - Mở file `esp32_robot_car/esp32_robot_car.ino`.
    - Chọn Board là **"ESP32 Dev Module"**.
    - Nhấn **Upload**.

## 3. Cách sử dụng Giao diện Qt

1.  **Biên dịch**: Mở `qt_interface/RobotControl.pro` bằng Qt Creator và chạy ứng dụng.
2.  **Kết nối**:
    - Nhấn **"Quét Thiết Bị"**.
    - Chọn **"RobotCar_ESP32_Unified"** và nhấn **"Kết Nối"**.
3.  **Điều khiển**:
    - Dùng các nút mũi tên (↑, ↓, ←, →) ở chế độ **Thủ Công**.
    - Nhấn **Tự Động** để xe tự dò line và tránh vật cản.

---
*Lưu ý: ESP32 sử dụng mức điện áp 3.3V. Đảm bảo các cảm biến và mạch driver được cấp nguồn phù hợp (thường L298N cấp nguồn 5V cho ESP32 qua chân VIN).*
