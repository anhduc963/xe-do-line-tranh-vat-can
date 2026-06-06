# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH (ESP32)

Hệ thống điều khiển xe tự hành tích hợp các tính năng dò line, tránh vật cản và điều khiển thủ công thông qua giao diện Bluetooth.

## 1. Thành phần phần cứng và Đấu nối

### Các linh kiện chính:
- **ESP32 Dev Module**: Trung tâm điều khiển.
- **L298N**: Mạch công suất điều khiển động cơ DC.
- **HC-SR04**: Cảm biến siêu âm đo khoảng cách.
- **TCRT5000**: 4 mắt cảm biến hồng ngoại dò line.
- **SG90**: Động cơ servo xoay cảm biến.
- **Encoder (Tùy chọn)**: Để đo tốc độ và quãng đường chính xác hơn.
- **Pin 18650 (2 viên)** & **Mạch hạ áp LM2596**: Cung cấp nguồn ổn định 5V cho ESP32 và 7.4V cho động cơ.

### Sơ đồ đấu nối GPIO:
1.  **Mạch điều khiển động cơ (L298N)**:
    - ENA: 32, IN1: 33, IN2: 25, IN3: 26, IN4: 27, ENB: 14
    - *Lưu ý*: Cần tháo 2 cái jumper (chốt nối) trên chân ENA và ENB của module L298N để kết nối với ESP32. Việc này giúp điều khiển tốc độ xe qua PWM.
2.  **Cảm biến siêu âm**:
    - Trig: 12, Echo: 13
3.  **Cảm biến dò line**:
    - LO: 34, LI: 35, RI: 36, RO: 39
4.  **Servo**: 15

## 2. Cài đặt và Nạp chương trình

1.  **Yêu cầu thư viện**:
    - Cài đặt thư viện **ESP32Servo** trong Arduino IDE.
2.  **Nạp chương trình**:
    - Mở file `esp32_robot_car/esp32_robot_car.ino`.
    - Chọn Board: **ESP32 Dev Module**.
    - Nhấn nạp code (Upload).

## 3. Giao diện điều khiển (Qt)

### Chạy ứng dụng:
1.  Mở `qt_interface/RobotControl.pro` bằng phần mềm Qt Creator.
2.  Biên dịch và khởi chạy ứng dụng.

### Cách điều khiển:
1.  **Kết nối**:
    - Nhấn nút **"Quét Thiết Bị"**.
    - Chọn thiết bị **"RobotCar_ESP32_Unified"** từ danh sách và nhấn **"Kết Nối"**.
2.  **Chế độ hoạt động**:
    - **Thủ Công**: Sử dụng các nút mũi tên (↑, ↓, ←, →) để lái xe và nút (■) để dừng.
    - **Tự Động**: Xe tự động thực hiện dò line và tránh vật cản nếu phát hiện phía trước có vật cản dưới 25cm.

## 4. Nâng cấp thêm (Encoder)
Nếu bạn lắp thêm Encoder, hãy nối chân tín hiệu vào các chân GPIO có hỗ trợ ngắt (Interrupt) như GPIO 18, 19. Việc này giúp xe chạy thẳng hơn thông qua thuật toán PID.

---
*Lưu ý: Luôn đảm bảo nguồn pin cung cấp đủ dòng điện cho cả động cơ và mạch ESP32 để hệ thống hoạt động ổn định nhất.*
