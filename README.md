# HƯỚNG DẪN ĐIỀU KHIỂN XE TỰ HÀNH (ESP32) - PHIÊN BẢN WIFI

Hệ thống điều khiển xe tự hành tích hợp các tính năng dò line, tránh vật cản và điều khiển thủ công thông qua giao diện WiFi (giao thức TCP).

## 1. Thành phần phần cứng và Đấu nối

### Các linh kiện chính:
- **ESP32 Dev Module**: Trung tâm điều khiển.
- **L298N**: Mạch công suất điều khiển động cơ DC.
- **HC-SR04**: Cảm biến siêu âm đo khoảng cách.
- **TCRT5000**: 4 mắt cảm biến hồng ngoại dò line.
- **SG90**: Động cơ servo xoay cảm biến (Góc xoay giới hạn 120 độ).
- **Pin 18650 (2 viên)** & **Mạch hạ áp LM2596**: Cung cấp nguồn ổn định 5V cho ESP32 và 7.4V cho động cơ.

### Sơ đồ đấu nối GPIO:
1.  **Mạch điều khiển động cơ (L298N)**:
    - ENA: 13, IN1: 16, IN2: 17, IN3: 18, IN4: 19, ENB: 14
    - *Lưu ý*: Cần tháo 2 cái jumper trên chân ENA và ENB để điều khiển tốc độ riêng biệt (`speeda` cho bên trái, `speedb` cho bên phải).
2.  **Cảm biến siêu âm**:
    - Trig: 23, Echo: 22
3.  **Cảm biến dò line**:
    - LO: 32, LI: 26, RI: 25, RO: 33
4.  **Servo**: 27

## 2. Giải thích các khái niệm C++ & Qt cơ bản (Dành cho người mới)

Trong mã nguồn Qt, bạn sẽ gặp các ký hiệu đặc biệt, dưới đây là ý nghĩa của chúng:

*   **`class` (Lớp)**: Giống như một bản thiết kế (ví dụ: bản thiết kế xe).
*   **`object` (Đối tượng)**: Là sản phẩm thực tế được tạo ra từ bản thiết kế (ví dụ: chiếc xe thật).
*   **`::` (Toán tử phạm vi)**: Dùng để chỉ ra một hàm thuộc về lớp nào.
*   **`this`**: Là con trỏ trỏ đến chính đối tượng hiện tại đang chạy code.
*   **`->` (Mũi tên)**: Dùng để truy cập vào các tính năng của một đối tượng khi ta đang dùng "Con trỏ".
*   **`*` (Dấu sao)**: Ký hiệu của "Con trỏ" (Pointer).
*   **`connect(...)`**: Cơ chế của Qt để kết nối sự kiện (như bấm nút) với hành động.

## 3. Cài đặt và Nạp chương trình

1.  **Yêu cầu thư viện**: Cài đặt thư viện **ESP32Servo** trong Arduino IDE.
2.  **Nạp chương trình**:
    - Mở file `wifi_robot_car_system/esp32_wifi_car/esp32_wifi_car.ino`.
    - Chọn Board: **ESP32 Dev Module**.
    - Nhấn nạp code (Upload).

## 4. Giao diện điều khiển (Sử dụng CMake)

Mã nguồn giao diện nằm trong thư mục `wifi_robot_car_system/qt_wifi_interface/`, sử dụng **CMake** và thư viện **Qt Network**.

### Cách biên dịch:
1.  Mở Qt Creator.
2.  Chọn **Open File or Project**.
3.  Chọn tệp `wifi_robot_car_system/qt_wifi_interface/CMakeLists.txt`.
4.  Cấu hình dự án với bộ Kit Qt hiện có của bạn.
5.  Nhấn nút **Run** (mũi tên xanh) để biên dịch và chạy.

### Cách điều khiển:
1.  **Kết nối WiFi**: Kết nối máy tính vào mạng WiFi do xe phát ra (Tên: **RobotCar_ESP32_WiFi**, không mật khẩu).
2.  **Giao diện**: Nhập IP `192.168.4.1` và Port `8080` (đã để mặc định) rồi nhấn **Kết Nối**.
2.  **Chế độ**:
    - **Thủ Công**: Sử dụng các phím mũi tên.
    - **Tự Động**: Xe thực hiện dò line và tránh vật cản (Sử dụng cảm biến siêu âm gắn trên Servo quét 120 độ).

---
*Lưu ý: Luôn đảm bảo nguồn pin cung cấp đủ dòng điện cho cả động cơ và mạch ESP32 để hệ thống hoạt động ổn định nhất.*
