#include "BluetoothSerial.h"
// #include <ESP32Servo.h> // Tam thoi bo qua

// Dinh nghia cac chan cho L298N tren ESP32
#define ENA 13
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENB 14

/*
// Dinh nghia cac chan cho cam bien sieu am HC-SR04 (Tam thoi bo)
#define TRIG 23
#define ECHO 22

// Dinh nghia chan cho Servo SG90 (Tam thoi bo)
#define SERVO_PIN 27
*/

// Dinh nghia cac chan cho cam bien do line TCRT5000
#define SENSOR_LO 32
#define SENSOR_LI 26
#define SENSOR_RI 25
#define SENSOR_RO 33

// Bien trang thai
char current_mode = 'M';

// Thiet lap toc do rieng cho tung ben dong co
float speeda = 140; // Toc do dong co A (Ben trai)
float speedb = 125; // Toc do dong co B (Ben phai)

// Servo myServo; // Tam thoi bo
BluetoothSerial SerialBT;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  /*
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  */

  pinMode(SENSOR_LO, INPUT); pinMode(SENSOR_LI, INPUT);
  pinMode(SENSOR_RI, INPUT); pinMode(SENSOR_RO, INPUT);

  /*
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(90);
  */

  SerialBT.begin("RobotCar_ESP32_Unified");
  Serial.begin(115200);

  stopCar();
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

void handleCommand(char cmd) {
  if (cmd == 'M' || cmd == 'A') {
    current_mode = cmd;
    stopCar();
  } else if (current_mode == 'M') {
    if (cmd == 'F') moveForward();
    else if (cmd == 'B') moveBackward();
    else if (cmd == 'L') turnLeft();
    else if (cmd == 'R') turnRight();
    else if (cmd == 'S') stopCar();
  }
}

// Giai thich chi tiet logic dieu khien dong co:
// Dong co A (Trai) duoc dieu khien boi IN1, IN2 va ENA
// Dong co B (Phai) duoc dieu khien boi IN3, IN4 va ENB
// HIGH/LOW quy dinh chieu quay, analogWrite quy dinh toc do (PWM)

void turnLeft() {
  // Logic re trai: Ca hai dong co cung quay theo mot chieu dac biet de xoay xe tai cho
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void turnRight() {
  // Logic re phai: Dao nguoc chieu quay so voi re trai
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveForward() {
  // Logic tien len: Hai dong co quay nguoc chieu nhau (tuy theo cach lap dat vat ly)
  // Trong code nay: Trai (IN1=LOW, IN2=HIGH), Phai (IN3=HIGH, IN4=LOW)
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveBackward() {
  // Logic lui lai: Dao nguoc toan bo trang thai cua tien len
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void autoDrive() {
  /*
  // Tam thoi bo logic vat can
  long distance = checkDistance();
  if (distance > 0 && distance < 25) {
     stopCar();
     return;
  }
  */
  lineFollowing();
}

/*
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
*/

void lineFollowing() {
  int lo = digitalRead(SENSOR_LO);
  int li = digitalRead(SENSOR_LI);
  int ri = digitalRead(SENSOR_RI);
  int ro = digitalRead(SENSOR_RO);

  if (li == HIGH && ri == HIGH) {
    moveForward();
  } else if (li == LOW && ri == HIGH) {
    turnRight();
  } else if (li == HIGH && ri == LOW) {
    turnLeft();
  } else if (lo == HIGH) {
    turnLeft();
  } else if (ro == HIGH) {
    turnRight();
  } else {
    // Khi tat ca cam bien deu o vung trang, duy tri toc do thap de tim line
    analogWrite(ENA, speeda / 2); analogWrite(ENB, speedb / 2);
  }
}
