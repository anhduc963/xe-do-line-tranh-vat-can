// ---------------------------------------------------------------------------
// GIAI THICH VE THU VIEN (#include):
// - WiFi: Dung de tao diem phat WiFi (Access Point) tu ESP32.
// ---------------------------------------------------------------------------
#include <WiFi.h>
#include <ESP32Servo.h>

// ---------------------------------------------------------------------------
// DINH NGHIA CHAN CAM (PIN DEFINITION):
// ---------------------------------------------------------------------------
#define ENA 13
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENB 14

#define TRIG 23
#define ECHO 22

#define SERVO_PIN 27

#define SENSOR_LO 32
#define SENSOR_LI 26
#define SENSOR_RI 25
#define SENSOR_RO 33

// ---------------------------------------------------------------------------
// CAU HINH WIFI:
// ---------------------------------------------------------------------------
const char* ssid = "RobotCar_ESP32_WiFi"; // Ten mang WiFi xe phat ra
const char* password = "";             // Mat khau (de trong neu khong can)
WiFiServer server(8080);               // Tao Server lang nghe o cong 8080

// ---------------------------------------------------------------------------
// BIEN TOAN CUC:
// ---------------------------------------------------------------------------
char current_mode = 'M';
float speeda = 140;
float speedb = 125;

Servo myServo;

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
  myServo.write(90);

  Serial.begin(115200);

  // Thiet lap WiFi o che do Access Point (AP)
  Serial.print("Dang khoi tao mang WiFi: ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dia chi IP cua xe: ");
  Serial.println(IP); // Mac dinh se la 192.168.4.1

  server.begin(); // Bat dau cho doi ket noi tu giao dien

  stopCar();
}

void loop() {
  // Kiem tra xem co giao dien (Client) nao dang ket noi khong
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Giao dien da ket noi!");
    while (client.connected()) {
      // Neu co du lieu truyen den
      if (client.available()) {
        char cmd = client.read();
        handleCommand(cmd);
      }

      // Chay logic tu dong neu dang o che do 'A'
      if (current_mode == 'A') {
        autoDrive();
      }
    }
    Serial.println("Giao dien da ngat ket noi.");
    stopCar(); // Dung xe khi ngat ket noi de an toan
  }

  // Truong hop khong co client ket noi nhung van muon chay tu dong (tuy chon)
  if (current_mode == 'A') {
    autoDrive();
  }
}

void handleCommand(char cmd) {
  if (cmd == 'M' || cmd == 'A') {
    current_mode = cmd;
    stopCar();
    myServo.write(90);
  }
  else if (current_mode == 'M') {
    if (cmd == 'F') moveForward();
    else if (cmd == 'B') moveBackward();
    else if (cmd == 'L') turnLeft();
    else if (cmd == 'R') turnRight();
    else if (cmd == 'S') stopCar();
  }
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveBackward() {
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
  long distance = checkDistance();

  if (distance > 0 && distance < 25) {
    stopCar();
    delay(200);
    myServo.write(30);
    delay(400);
    long distRight = checkDistance();
    myServo.write(150);
    delay(600);
    long distLeft = checkDistance();
    myServo.write(90);
    delay(400);

    if (distRight > distLeft) {
      turnRight();
      delay(400);
    } else {
      turnLeft();
      delay(400);
    }
  }
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

void lineFollowing() {
  int lo = digitalRead(SENSOR_LO);
  int li = digitalRead(SENSOR_LI);
  int ri = digitalRead(SENSOR_RI);
  int ro = digitalRead(SENSOR_RO);

  if (li == HIGH && ri == HIGH) {
    moveForward();
  }
  else if (li == LOW && ri == HIGH) {
    turnRight();
  }
  else if (li == HIGH && ri == LOW) {
    turnLeft();
  }
  else if (lo == HIGH) {
    turnLeft();
  }
  else if (ro == HIGH) {
    turnRight();
  }
  else {
    analogWrite(ENA, speeda / 2); analogWrite(ENB, speedb / 2);
  }
}
