#include "mat.h"

void setup() {
  
  mat_init();

  #ifdef DEBUG
    Serial.println("Start");
    delay(500);
  #endif
  
}

// ========================
// loop() Section
// ========================
void loop() {
  mat_loop();
}

/*
Suggested Improvements:

    Add Timeouts:
    cpp

unsigned long stateStartTime;
if (currentTime - stateStartTime > 5000) { // 5s timeout
  triggerError();
}

Difficulty Levels:
cpp

int requiredHoldTime = 1000; // Adjustable difficulty

Scoring System:
cpp

    int score = 0;
    void advanceStep() {
      score += 10;
      // ... rest of function
    }

This 

Add Serial Commands (for advanced control):
cpp

if (Serial.available()) {
  char cmd = Serial.read();
  if (cmd == 'N') setProgram((currentProgram + 1) % TRAININGS);
  else if (cmd == 'R') currentState = IDLE;
}

EEPROM Storage (for program persistence):
cpp

#include <EEPROM.h>
void saveSettings() {
  EEPROM.update(0, currentProgram);
}

Power Management:
cpp

    void enterLowPower() {
      if (currentState == IDLE && millis() - lastActivity > 300000) {
        // Deep sleep after 5 minutes idle
      }
    }

Would you like me to:

*/