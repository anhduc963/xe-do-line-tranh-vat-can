#include "BluetoothSerial.h"
#include <ESP32Servo.h> // Da khoi phuc thu vien Servo

// ---------------------------------------------------------------------------
// GIAI THICH VE THU VIEN (#include):
// - BluetoothSerial: Dung de bien ESP32 thanh mot thiet bi Bluetooth,
//   nhan lenh tu dien thoai hoac may tinh nhu mot cong Serial khong day.
// - ESP32Servo: Thu vien dieu khien goc quay cua Servo SG90.
// ---------------------------------------------------------------------------
#include "BluetoothSerial.h"
#include <ESP32Servo.h>

// ---------------------------------------------------------------------------
// DINH NGHIA CHAN CAM (PIN DEFINITION):
// #define giup dat ten cho cac so chan, giup code de doc va de sua doi sau nay.
// ---------------------------------------------------------------------------
// Chan dieu khien dong co L298N
#define ENA 13  // Chan PWM dieu khien toc do dong co A (Ben Trai)
#define IN1 16  // Chan huong dong co A
#define IN2 17  // Chan huong dong co A
#define IN3 18  // Chan huong dong co B
#define IN4 19  // Chan huong dong co B
#define ENB 14  // Chan PWM dieu khien toc do dong co B (Ben Phai)

// Chan cam bien sieu am HC-SR04 (Dung de do khoang cach vat can)
#define TRIG 23 // Chan phat song am
#define ECHO 22 // Chan thu song am phan hoi

// Chan Servo SG90 (Xoay mat cam bien sieu am)
#define SERVO_PIN 27

// Chan cam bien do line TCRT5000 (Phat hien vach den/trang)
#define SENSOR_LO 32 // Ngoai cung ben trai
#define SENSOR_LI 26 // Trong ben trai
#define SENSOR_RI 25 // Trong ben phai
#define SENSOR_RO 33 // Ngoai cung ben phai

// ---------------------------------------------------------------------------
// KHAI BAO BIEN TOAN CUC:
// Bien toan cuc co the duoc su dung o bat ky dau trong chuong trinh.
// ---------------------------------------------------------------------------
char current_mode = 'M'; // 'M' = Manual (Thu cong), 'A' = Auto (Tu dong)

// Toc do ban dau cho 2 ben dong co (Co the thay doi de xe di thang hon)
float speeda = 140;
float speedb = 125;

Servo myServo;
BluetoothSerial SerialBT;

/**
 * HAM SETUP: Chay 1 lan duy nhat khi ESP32 moi khoi dong.
 * Dung de thiet lap trang thai ban dau cho cac chan cam.
 */
void setup() {
  // Thiet lap cac chan dong co la dau RA (OUTPUT)
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  // Thiet lap chan sieu am
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  // Thiet lap chan cam bien do line la dau VAO (INPUT)
  pinMode(SENSOR_LO, INPUT); pinMode(SENSOR_LI, INPUT);
  pinMode(SENSOR_RI, INPUT); pinMode(SENSOR_RO, INPUT);

  // Khoi tao Servo (Cap phat bo dinh thoi gian PWM cho ESP32)
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50); // Servo tieu chuan chay o tan so 50Hz
  myServo.attach(SERVO_PIN, 500, 2400); // Gan chan va thiet lap xung (min, max)
  myServo.write(90); // Cho mat cam bien nhin thang vao giua

  // Bat dau giao tiep Bluetooth voi ten hien thi tren dien thoai
  SerialBT.begin("RobotCar_ESP32_Unified");
  Serial.begin(115200); // Bat cong Serial de debug qua may tinh

  stopCar(); // Dam bao xe khong tu chay khi moi bat
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    handleCommand(cmd);
  }

  if (current_mode == 'A') {
    autoDrive();
  }
}

/**
 * HAM HANDLECOMMAND: Xu ly cac ky tu nhan duoc tu Bluetooth.
 * Moi ky tu tuong ung voi mot hanh dong khac nhau.
 */
void handleCommand(char cmd) {
  // Chuyen doi che do: M = Thu cong, A = Tu dong
  if (cmd == 'M' || cmd == 'A') {
    current_mode = cmd;
    stopCar();
    myServo.write(90);
  }
  // Neu dang o che do thu cong, thuc hien lenh dieu khien truc tiep
  else if (current_mode == 'M') {
    if (cmd == 'F') moveForward();      // Di thang
    else if (cmd == 'B') moveBackward(); // Lui lai
    else if (cmd == 'L') turnLeft();     // Quay trai
    else if (cmd == 'R') turnRight();    // Quay phai
    else if (cmd == 'S') stopCar();      // Dung lai
  }
}

// ---------------------------------------------------------------------------
// CAC HAM DIEU KHIEN DONG CO:
// Su dung digitalWrite de set huong va analogWrite de set toc do (0-255).
// ---------------------------------------------------------------------------

void turnLeft() {
  // Quay trai: Ban trai lui, ban phai tien (Xoay tai cho)
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void turnRight() {
  // Quay phai: Ban trai tien, ban phai lui (Xoay tai cho)
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveForward() {
  // Di thang: Ca hai ban cung tien
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveBackward() {
  // Lui lai: Ca hai ban cung lui
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void stopCar() {
  // Dung lai: Ngat dien tat ca cac chan
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

/**
 * HAM AUTODRIVE: Che do tu hanh.
 * Ket hop giua tranh vat can (uu tien cao) va do line.
 */
void autoDrive() {
  // Kiem tra khoang cach truoc mat
  long distance = checkDistance();

  // Neu co vat can gan hon 25cm
  if (distance > 0 && distance < 25) {
    stopCar();
    delay(200);

    // Xoay cam bien sang phai de nhin
    myServo.write(30);
    delay(400);
    long distRight = checkDistance();

    // Xoay cam bien sang trai de nhin
    myServo.write(150);
    delay(600);
    long distLeft = checkDistance();

    // Tra cam bien ve giua
    myServo.write(90);
    delay(400);

    // So sanh xem ben nao thoang hon thi re vao
    if (distRight > distLeft) {
      turnRight();
      delay(400);
    } else {
      turnLeft();
      delay(400);
    }
  }
  // Neu duong thoang thi tiep tuc do line
  else {
    lineFollowing();
  }
}

long checkDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 0;
  return duration * 0.034 / 2;
}

/**
 * HAM LINEFOLLOWING: Dieu khien xe bam theo vach den.
 * HIGH thuong la phep den (Tuy vao loai cam bien).
 */
void lineFollowing() {
  int lo = digitalRead(SENSOR_LO); // Ben trai ngoai
  int li = digitalRead(SENSOR_LI); // Ben trai trong
  int ri = digitalRead(SENSOR_RI); // Ben phai trong
  int ro = digitalRead(SENSOR_RO); // Ben phai ngoai

  // Neu 2 mat giua deu thay vach -> Di thang
  if (li == HIGH && ri == HIGH) {
    moveForward();
  }
  // Neu mat phai thay vach -> Re phai
  else if (li == LOW && ri == HIGH) {
    turnRight();
  }
  // Neu mat trai thay vach -> Re trai
  else if (li == HIGH && ri == LOW) {
    turnLeft();
  }
  // Neu mat ngoai cung ben trai thay vach -> Re trai gap
  else if (lo == HIGH) {
    turnLeft();
  }
  // Neu mat ngoai cung ben phai thay vach -> Re phai gap
  else if (ro == HIGH) {
    turnRight();
  }
  // Truong hop khong thay vach -> Di cham lai de tim vach
  else {
    analogWrite(ENA, speeda / 2); analogWrite(ENB, speedb / 2);
  }
}
