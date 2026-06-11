# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH (ESP32)

Hệ thống điều khiển xe tự hành tích hợp các tính năng dò line và điều khiển thủ công thông qua giao diện Bluetooth.

## 1. Thành phần phần cứng và Đấu nối

### Các linh kiện chính:
- **ESP32 Dev Module**: Trung tâm điều khiển.
- **L298N**: Mạch công suất điều khiển động cơ DC.
- **TCRT5000**: 4 mắt cảm biến hồng ngoại dò line.
- **Pin 18650 (2 viên)** & **Mạch hạ áp LM2596**: Cung cấp nguồn ổn định 5V cho ESP32 và 7.4V cho động cơ.

### Sơ đồ đấu nối GPIO:
1.  **Mạch điều khiển động cơ (L298N)**:
    - ENA: 13, IN1: 16, IN2: 17, IN3: 18, IN4: 19, ENB: 14
    - *Lưu ý*: Cần tháo 2 cái jumper trên chân ENA và ENB để điều khiển tốc độ riêng biệt (`speeda` cho bên trái, `speedb` cho bên phải).
2.  **Cảm biến dò line**:
    - LO: 32, LI: 26, RI: 25, RO: 33

*(Lưu ý: Các linh kiện như Cảm biến siêu âm và Servo hiện đang được tạm thời vô hiệu hóa trong mã nguồn để tập trung vào logic động cơ).*

## 2. Cài đặt và Nạp chương trình

1.  **Nạp chương trình**:
    - Mở file `esp32_robot_car/esp32_robot_car.ino`.
    - Chọn Board: **ESP32 Dev Module**.
    - Nhấn nạp code (Upload).

## 3. Giao diện điều khiển (Qt)

Mã nguồn Qt đã được chú thích CHI TIẾT từng dòng để bạn dễ dàng tìm hiểu.
Bạn có thể mở dự án bằng Qt Creator qua file `qt_interface/RobotControl.pro` hoặc `qt_interface/CMakeLists.txt`.

### Cách điều khiển:
1.  **Kết nối**: Quét và kết nối tới thiết bị **"RobotCar_ESP32_Unified"**.
2.  **Chế độ**:
    - **Thủ Công**: Sử dụng các phím mui tên.
    - **Tự Động**: Xe thực hiện dò line với logic động cơ đã được tối ưu (speeda=140, speedb=125).

---
*Lưu ý: Luôn đảm bảo nguồn pin cung cấp đủ dòng điện cho cả động cơ và mạch ESP32 để hệ thống hoạt động ổn định nhất.*
