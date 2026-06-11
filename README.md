# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH (ESP32)

Hệ thống điều khiển xe tự hành tích hợp các tính năng dò line, tránh vật cản và điều khiển thủ công thông qua giao diện Bluetooth.

## 1. Thành phần phần cứng và Đấu nối

### Các linh kiện chính:
- **ESP32 Dev Module**: Trung tâm điều khiển.
- **L298N**: Mạch công suất điều khiển động cơ DC.
- **HC-SR04**: Cảm biến siêu âm đo khoảng cách.
- **TCRT5000**: 4 mắt cảm biến hồng ngoại dò line.
- **SG90**: Động cơ servo xoay cảm biến (Góc xoay giới hạn 120 độ).
- **Pin 18650 (2 viên)** & **Mạch hạ áp LM2596**: Cung cấp nguồn ổn định 5V cho ESP32 và 7.4V cho động cơ.

### Sơ đồ đấu nối GPIO (Theo yêu cầu gốc):
1.  **Mạch điều khiển động cơ (L298N)**:
    - ENA: 13, IN1: 16, IN2: 17, IN3: 18, IN4: 19, ENB: 14
    - *Lưu ý*: Cần tháo 2 cái jumper trên chân ENA và ENB của module L298N để kết nối với ESP32 nhằm điều khiển tốc độ qua PWM.
2.  **Cảm biến siêu âm**:
    - Trig: 23, Echo: 22
3.  **Cảm biến dò line**:
    - LO: 32, LI: 26, RI: 25, RO: 33
4.  **Servo**: 27

## 2. Cài đặt và Nạp chương trình

1.  **Yêu cầu thư viện**:
    - Cài đặt thư viện **ESP32Servo** trong Arduino IDE.
2.  **Nạp chương trình**:
    - Mở file `esp32_robot_car/esp32_robot_car.ino`.
    - Chọn Board: **ESP32 Dev Module**.
    - Nhấn nạp code (Upload).

## 3. Giao diện điều khiển (Qt)

Mã nguồn Qt đã được chú thích CHI TIẾT từng dòng để bạn dễ dàng tìm hiểu.
Bạn có thể mở dự án bằng Qt Creator theo hai cách:
1.  **QMake**: Mở file `qt_interface/RobotControl.pro`.
2.  **CMake**: Mở file `qt_interface/CMakeLists.txt`.

### Cách điều khiển:
1.  **Kết nối**:
    - Nhấn nút **"Quét Thiết Bị"**.
    - Chọn thiết bị **"RobotCar_ESP32_Unified"** từ danh sách và nhấn **"Kết Nối"**.
2.  **Chế độ hoạt động**:
    - **Thủ Công**: Sử dụng các nút mũi tên (↑, ↓, ←, →) để lái xe.
    - **Tự Động**: Xe tự động dò line và tránh vật cản. Khi gặp vật cản, servo sẽ quét một góc 120 độ để tìm đường đi.

---
*Lưu ý: Luôn đảm bảo nguồn pin cung cấp đủ dòng điện cho cả động cơ và mạch ESP32 để hệ thống hoạt động ổn định nhất.*
