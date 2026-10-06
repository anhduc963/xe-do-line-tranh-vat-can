#include <WiFi.h>
#include <ESP32Servo.h>

// Dinh nghia chan cam
//Dong co 
#define ENA 13
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENB 14
//Cam bien sieu am
#define TRIG 23
#define ECHO 22
//Servo
#define SERVO_PIN 27
//Cam bien hong ngoai
#define SENSOR_LO 26
#define SENSOR_LI 25
#define SENSOR_RI 33
#define SENSOR_RO 32

//Cau hinh wifi
const char* ssid = "RobotCar_ESP32_WiFi"; 
const char* password = "";             
WiFiServer server(8080);               

//Bien toan cuc
char current_mode = 'M';
int speeda = 120;
int speedb = 130;
char last_action = 'S';

Servo myServo;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  pinMode(SENSOR_LO, INPUT); pinMode(SENSOR_LI, INPUT);
  pinMode(SENSOR_RI, INPUT); pinMode(SENSOR_RO, INPUT);

  ESP32PWM::allocateTimer(0);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(90);

  Serial.begin(115200);

  // Thiet lap WiFi 
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dia chi IP cua xe: ");
  Serial.println(IP); // Mac dinh se la 192.168.4.1

  server.begin(); // Bat dau cho doi ket noi tu giao dien

  stopCar();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      // Neu co du lieu truyen den
      if (client.available()) {
        char cmd = client.read();
        handleCommand(cmd);
      }

      if (current_mode == 'A') {
        autoDrive();
      }
    }
    stopCar(); 
  }
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
  else if (cmd == 'S'){          // Đưa ra ngoài nhóm 'M'
    stopCar();
    current_mode = 'M';          // Ép xe dừng và trả về thủ công cho an toàn
  }
  else if (current_mode == 'M') {
    if      (cmd == 'F') moveForward();
    else if (cmd == 'B') moveBackward();
    else if (cmd == 'L') turnLeft();
    else if (cmd == 'R') turnRight();
    else if (cmd == 'S') stopCar();
  }
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
  
  if (current_mode == 'M') {
    delay(300); 
    resumeLastAction(); 
  }
}

void resumeLastAction() {
  if (last_action == 'F') {
    moveForward();
  } else if (last_action == 'B') {
    moveBackward();
  } else {
    stopCar();
  }
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
  
  if (current_mode == 'M') {
    delay(300); 
    resumeLastAction(); 
  }
}

void moveBackward () {
  last_action = 'B';
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}

void moveForward () {
  last_action = 'F';
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speeda); analogWrite(ENB, speedb);
}


void stopCar() {
  last_action = 'S';
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
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

  if (li == HIGH && ri == HIGH)     moveForward(); 
  else if (li == LOW && ri == HIGH) turnRight(); 
  else if (li == HIGH && ri == LOW) turnLeft(); 
  else if (lo == HIGH)              turnLeft(); 
  else if (ro == HIGH)              turnRight();
  else{
    stopCar();
    current_mode ='M';
    myServo.write(90);
  }
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
      current_mode = 'M';
    } else {
      turnLeft();
      delay(400); 
      current_mode = 'M';
    }
    stopCar(); 
  }
  else {
    lineFollowing();
  }
}
