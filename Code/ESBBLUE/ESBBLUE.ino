#include "blue.h"

bool program_flag = true;
char cmd;
void setup() {
  // put your setup code here, to run once:
  BLUE_init();
  BLUE_Start();
  pinMode(2,OUTPUT);
}

void loop() {
  if (Bluetooth_serialESP.available()) {
    cmd = Bluetooth_serialESP.read();  // Read one character

    if (cmd == '1') {
      digitalWrite(2, HIGH);  // Turn LED ON
      BLUE_Start();
    }

    else if (cmd == '0') {
      digitalWrite(2, LOW);   // Turn LED OFF
      Bluetooth_serialESP.println("LED is OFF ❌");
      BLUE_Problem_intro(3);
    }

    else {
      Bluetooth_serialESP.println("Unknown command ❓");
    }
  }

  delay(100);  // Just to avoid flooding
}

