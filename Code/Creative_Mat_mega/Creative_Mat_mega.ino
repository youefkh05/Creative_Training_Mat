#include "mat.h"

void setup() {
  Serial.begin(9600);
  mat_init();
  Serial.println("Start");
  delay(500);
}

// ========================
// loop() Section
// ========================
void loop() {
  unsigned long currentTime = millis();
  
  switch(currentState) {
    case IDLE:        handleIdle(); break;
    case BLINK_TARGET: handleBlinkTarget(currentTime); break;
    case WAIT_FOR_HOLD: handleWaitForHold(currentTime); break;
    case VERIFY_NEXT: handleVerifyNext(); break;
  }
  
  mat_checkerror();

  #ifdef DEBUG
    printState();
    delay(100); // Prevent serial flooding
  #endif
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