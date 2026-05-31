# HỆ THỐNG ĐIỀU KHIỂN XE TỰ HÀNH (ARDUINO + ESP32 + QT)

Dự án này bao gồm toàn bộ mã nguồn cho xe tự hành có các tính năng:
- **Dò line (Line Following)**
- **Tránh vật cản (Obstacle Avoidance)**
- **Điều khiển thủ công qua giao diện Qt (C++)**

## 1. Thành phần phần cứng
- **Arduino Uno R3**: Xử lý logic điều khiển động cơ và cảm biến.
- **ESP32**: Làm cầu nối WiFi (nhận lệnh UDP từ Qt và gửi qua Serial tới Arduino).
- **L298N**: Mạch công suất điều khiển động cơ DC.
- **TCRT5000 (4 cái)**: Cảm biến hồng ngoại dò vạch.
- **HC-SR04**: Cảm biến siêu âm đo khoảng cách.
- **SG90**: Động cơ servo xoay cảm biến siêu âm.
- **Encoder**: Có thể lắp thêm vào bánh xe (kết nối với chân 2, 3 của Arduino).

## 2. Sơ đồ đấu nối chính

### Arduino Uno & L298N
- ENA -> 5, IN1 -> 7, IN2 -> 8
- IN3 -> 9, IN4 -> 11, ENB -> 6

### Cảm biến
- Siêu âm: Trig -> 12, Echo -> 13
- Servo: Signal -> 10
- Dò line: LO -> A0, LI -> A1, RI -> A2, RO -> A3

### Giao tiếp Arduino Uno & ESP32
- Uno RX (0) <--> ESP32 TX (17)
- Uno TX (1) <--> ESP32 RX (16)
- **GND chung** giữa 2 board.

## 3. Cấu trúc mã nguồn
- `arduino_car/`: Chứa code cho Arduino Uno (xử lý real-time).
- `esp32_bridge/`: Chứa code cho ESP32 (làm WiFi AP & UDP bridge cho ứng dụng Qt).
- `esp32_bluetooth_bridge/`: Chứa code cho ESP32 để điều khiển qua Bluetooth bằng điện thoại.
- `qt_interface/`: Chứa mã nguồn C++ sử dụng framework Qt cho giao diện điều khiển trên máy tính.

## 4. Hướng dẫn sử dụng

### Cách 1: Điều khiển qua WiFi (Dùng máy tính với Qt)
1. Nạp `arduino_car.ino` vào Arduino Uno.
2. Nạp `esp32_bridge.ino` vào ESP32.
3. Kết nối máy tính vào WiFi `RobotCar_Control` (mật khẩu `password123`).
4. Chạy ứng dụng Qt trên máy tính để điều khiển.

### Cách 2: Điều khiển qua Bluetooth (Dùng điện thoại)
1. Nạp `arduino_car.ino` vào Arduino Uno.
2. Nạp `esp32_bluetooth_bridge.ino` vào ESP32.
3. Trên điện thoại, bật Bluetooth và ghép nối với thiết bị tên `RobotCar_Bluetooth`.
4. Sử dụng các ứng dụng như "Serial Bluetooth Terminal" để gửi các ký tự điều khiển:
   - `F`: Tiến, `B`: Lùi, `L`: Trái, `R`: Phải, `S`: Dừng.
   - `M`: Chế độ thủ công, `A`: Dò line, `O`: Tránh vật cản.

*Lưu ý: Luôn rút dây nối Serial (0, 1) khi nạp code cho Arduino Uno.*
