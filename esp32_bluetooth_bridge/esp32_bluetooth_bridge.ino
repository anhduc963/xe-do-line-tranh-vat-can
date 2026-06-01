#include "BluetoothSerial.h"

// # Kiem tra xem Bluetooth co duoc bat khong
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  // # Thiet lap Serial de giao tiep voi Arduino Uno
  // # Su dung Serial2 (TX17, RX16) de khong trung voi Serial USB
  Serial.begin(115200); // Dung cho debug qua USB
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // Ket noi voi Arduino Uno

  // # Thiet lap Bluetooth voi ten thiet bi
  SerialBT.begin("RobotCar_Bluetooth");

  // # Thong bao san sang
  // Serial.println("Thiet bi da san sang de ket noi Bluetooth!");
}

void loop() {
  // # Doc du lieu tu Bluetooth va gui sang Arduino qua Serial2
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    Serial2.write(incomingChar);
    Serial.print("Bluetooth -> Arduino: ");
    Serial.println(incomingChar);
  }

  // # Doc du lieu tu Arduino (neu co) va gui nguoc lai Bluetooth
  if (Serial2.available()) {
    char incomingChar = Serial2.read();
    SerialBT.write(incomingChar);
    Serial.print("Arduino -> Bluetooth: ");
    Serial.println(incomingChar);
  }

  delay(20);
}
