#include "BluetoothSerial.h"

BluetoothSerial Bluetooth_serialESP;
char cmd;
void setup() {
  // put your setup code here, to run once:
  Bluetooth_serialESP.begin("ESP32-BT");
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Bluetooth_serialESP.available()){
    cmd = Bluetooth_serialESP.read();
  }
  else{

  }
  if(cmd  ==  '1'){
    digitalWrite(2,HIGH);
  }
  if(cmd  ==  '0'){
    digitalWrite(2,LOW);
  }
  delay(100);
}
