#include <Servo.h>

// # Dinh nghia cac chan ket noi cho L298N
#define ENA 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENB 6

// # Dinh nghia cac chan cho cam bien sieu am HC-SR04
#define TRIG 12
#define ECHO 13

// # Dinh nghia chan cho Servo SG90
#define SERVO_PIN 10

// # Dinh nghia cac chan cho cam bien do line TCRT5000
#define SENSOR_LO A0 // Left Outer
#define SENSOR_LI A1 // Left Inner
#define SENSOR_RI A2 // Right Inner
#define SENSOR_RO A3 // Right Outer

// # Bien trang thai
char current_mode = 'M'; // M: Manual, A: Auto (Line + Obstacle)
int speed = 150;
Servo myServo;

void setup() {
  // # Thiet lap cac chan dau ra cho dong co
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  // # Thiet lap cac chan cho sieu am
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  // # Thiet lap cac chan cho do line
  pinMode(SENSOR_LO, INPUT); pinMode(SENSOR_LI, INPUT);
  pinMode(SENSOR_RI, INPUT); pinMode(SENSOR_RO, INPUT);

  // # Thiet lap servo
  myServo.attach(SERVO_PIN);
  myServo.write(90); // Huong thang ve phia truoc

  // # Thiet lap giao tiep Serial de nhan lenh tu ESP32
  Serial.begin(9600);

  // # Dung xe luc moi bat dau
  stopCar();
}

void loop() {
  // # Kiem tra lenh tu Serial (ESP32)
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    handleCommand(cmd);
  }

  // # Thuc hien hanh dong theo che do hien tai
  if (current_mode == 'A') {
    autoDrive();
  }
}

void handleCommand(char cmd) {
  // # Chuyen doi che do hoac dieu khien huong
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

// # Ham tu dong: Ket hop do line va tranh vat can
void autoDrive() {
  long distance = checkDistance();

  // # Neu co vat can gan (duoi 25cm)
  if (distance > 0 && distance < 25) {
    stopCar();
    delay(200);
    moveBackward();
    delay(300);
    turnRight();
    delay(400);
  } else {
    // # Neu khong co vat can thi do line
    lineFollowing();
  }
}

// # Ham do khoang cach bang cam bien sieu am
long checkDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000); // Timeout 30ms
  if (duration == 0) return 0;
  return duration * 0.034 / 2;
}

// # Ham do line
void lineFollowing() {
  int lo = digitalRead(SENSOR_LO);
  int li = digitalRead(SENSOR_LI);
  int ri = digitalRead(SENSOR_RI);
  int ro = digitalRead(SENSOR_RO);

  // # Logic do line co ban (1 la gap vach den, 0 la nen trang)
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
    // # Di cham neu mat line
    analogWrite(ENA, 100); analogWrite(ENB, 100);
  }
}

// # Ham dieu khien dong co
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
