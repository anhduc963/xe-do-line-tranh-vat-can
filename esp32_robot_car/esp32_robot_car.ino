#include "BluetoothSerial.h"
#include <ESP32Servo.h> // Can cai thu vien ESP32Servo

// Dinh nghia cac chan cho L298N tren ESP32
#define ENA 32
#define IN1 19
#define IN2 21
#define IN3 22
#define IN4 23
#define ENB 14

// Dinh nghia cac chan cho cam bien sieu am HC-SR04
#define TRIG 5
#define ECHO 18

// Dinh nghia chan cho Servo SG90
#define SERVO_PIN 15

// Dinh nghia cac chan cho cam bien do line TCRT5000
#define SENSOR_LO 27
#define SENSOR_LI 26
#define SENSOR_RI 25
#define SENSOR_RO 33

// Bien trang thai
char current_mode = 'M';
int speed = 200;
Servo myServo;
BluetoothSerial SerialBT;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(SENSOR_LO, INPUT); pinMode(SENSOR_LI, INPUT);
  pinMode(SENSOR_RI, INPUT); pinMode(SENSOR_RO, INPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);

  // Dat servo o vi tri chinh giua (90 do)
  myServo.write(90);

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
    myServo.write(90); // Reset servo ve giua khi chuyen che do
  } else if (current_mode == 'M') {
    if (cmd == 'F') moveForward();
    else if (cmd == 'B') moveBackward();
    else if (cmd == 'L') turnLeft();
    else if (cmd == 'R') turnRight();
    else if (cmd == 'S') stopCar();
  }
}

void autoDrive() {
  long distance = checkDistance();

  if (distance > 0 && distance < 25) {
    stopCar();
    delay(200);

    // Quet cam bien trong khoang 120 do (tu 30 den 150)
    myServo.write(30);
    delay(400);
    long distRight = checkDistance();

    myServo.write(150);
    delay(600);
    long distLeft = checkDistance();

    myServo.write(90); // Ve lai giua
    delay(400);

    if (distRight > distLeft) {
      turnRight();
      delay(400);
    } else {
      turnLeft();
      delay(400);
    }
  } else {
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
    analogWrite(ENA, speed / 2); analogWrite(ENB, speed / 2);
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}
