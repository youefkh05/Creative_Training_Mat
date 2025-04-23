// State Machine
enum States {
  IDLE,          // New state
  BLINK_TARGET,
  WAIT_FOR_HOLD, 
  VERIFY_NEXT,
  ERROR
};

#ifdef DEBUG
void printState() {
  Serial.print("State: ");
  switch(currentState) {
    case IDLE: Serial.print("IDLE"); break;
    case BLINK_TARGET: Serial.print("BLINK_TARGET"); break;
    case WAIT_FOR_HOLD: Serial.print("WAIT_FOR_HOLD"); break;
    case VERIFY_NEXT: Serial.print("VERIFY_NEXT"); break;
    case ERROR: Serial.print("ERROR"); break;
  }
  Serial.print(" | Program: ");
  Serial.print(currentProgram);
  Serial.print(" | Step: ");
  Serial.print(programStep);
  Serial.print(" | Current: ");
  Serial.print(currentStep);
  Serial.print(" | Next: ");
  Serial.println(nextStep);
}
#endif


// ========================
// 1. Define Training Programs
// ========================
#define TRAININGS  3  // Number of available programs
#define MAX_STEPS  5  // Maximum steps per program
#define MAX_LEDS   5  // Number of physical LEDs per color
#define RED_OFFSET MAX_LEDS  // Red LEDs start at index MAX_LEDS

// Add debounce constants
#define DEBOUNCE_DELAY  50
#define HOLD_DURATION   500
#define BLINK_INTERVAL  500
#define ERROR_DURATION  2000

// Pin Definitions
const int BUTTONS[] = {0, 1, 2, 3, A0};  // B1-B5 (HIGH when pressed)

// Combined LED array (first MAX_LEDS are green, next MAX_LEDS are red)
const int ALL_LEDS[] = {4,5,6,7,8, 9,10,11,12,13}; // [0-4]=Green, [5-9]=Red

// Exercise sequences for each program
const int PROGRAMS[TRAININGS][MAX_STEPS] = {
  {0, 1, 2, 3, 4},              // Program 0: All green
  {1, 2+RED_OFFSET, 1, 3, -1},  // Program 1: Step 2 uses red
  {4+RED_OFFSET, 3, 2, -1, -1}  // Program 2: Step 0 uses red
};

States currentState = IDLE;     // Start in idle
int currentProgram = 0;
int programStep = 0;
int totalSteps = MAX_STEPS;     // Will be adjusted per program
int currentStep = PROGRAMS[currentProgram][0];
int nextStep = -1;
unsigned long lastBlinkTime = 0;
bool ledState = false;
unsigned long errorStartTime = 0;


void setup() {
  // Initialize all LEDs and buttons
  for (int i = 0; i < MAX_LEDS; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(ALL_LEDS[i], OUTPUT);          // Green LEDs
    pinMode(ALL_LEDS[i + RED_OFFSET], OUTPUT); // Red LEDs
    digitalWrite(ALL_LEDS[i], LOW);
    digitalWrite(ALL_LEDS[i + RED_OFFSET], LOW);
  }
}

// ========================
// LED Control Functions
// ========================

void blinkLed(int step, unsigned long currentTime) {
  if (currentTime - lastBlinkTime >= 500) {
    ledState = !ledState;
    int physicalPin = ALL_LEDS[step];
    digitalWrite(physicalPin, ledState);
    
    // Turn off opposite color
    int oppositeColorPin = ALL_LEDS[(step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET];
    digitalWrite(oppositeColorPin, LOW);
    
    lastBlinkTime = currentTime;
  }
}

void setLedSolid(int step) {
  int physicalPin = ALL_LEDS[step];
  digitalWrite(physicalPin, HIGH);
  
  // Turn off opposite color
  int oppositeColorPin = ALL_LEDS[(step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET];
  digitalWrite(oppositeColorPin, LOW);
}

void resetAllLEDs() {
  for (int i = 0; i < MAX_LEDS*2; i++) {
    digitalWrite(ALL_LEDS[i], LOW);
  }
}

// Improved button reading with debounce
bool isButtonPressed(int buttonIndex) {
  static unsigned long lastDebounceTime[MAX_LEDS] = {0};
  static bool lastButtonState[MAX_LEDS] = {LOW};
  
  bool reading = digitalRead(BUTTONS[buttonIndex]);
  if (reading != lastButtonState[buttonIndex]) {
    lastDebounceTime[buttonIndex] = millis();
  }
  
  if ((millis() - lastDebounceTime[buttonIndex]) > DEBOUNCE_DELAY) {
    if (reading != buttonStates[buttonIndex]) {
      buttonStates[buttonIndex] = reading;
      return reading;
    }
  }
  lastButtonState[buttonIndex] = reading;
  return false;
}

// ========================
// State Handlers
// ========================

void handleIdle() {
  if (digitalRead(BUTTONS[0]) == HIGH) {
    setProgram(currentProgram);
  }
}

void handleBlinkTarget(unsigned long currentTime) {
  blinkLed(currentStep, currentTime);
  
  if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == HIGH) {
    setLedSolid(currentStep);
    currentState = WAIT_FOR_HOLD;
  }
}

void handleWaitForHold(unsigned long currentTime) {
  if (programStep + 1 < totalSteps) {
    nextStep = PROGRAMS[currentProgram][programStep + 1];
  } else {
    nextStep = -1;
  }

  if (nextStep != -1) {
    setLedSolid(currentStep);
    blinkLed(nextStep, currentTime);

    if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == LOW) {
      triggerError(currentState);
    } 
    else if (digitalRead(BUTTONS[nextStep % MAX_LEDS]) == HIGH) {
      setLedSolid(nextStep);
      currentState = VERIFY_NEXT;
    }
  } else {
    advanceStep();
  }
}

void handleVerifyNext() {
  delay(500);
  if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == HIGH && 
      digitalRead(BUTTONS[nextStep % MAX_LEDS]) == HIGH) {
    advanceStep();
  } else {
    triggerError(currentState);
  }
}

void handleError(unsigned long currentTime) {
  // Blink current step's red LED
  int errorLed = (currentStep % MAX_LEDS) + RED_OFFSET;
  blinkLed(errorLed, currentTime);
  
  if (millis() - errorStartTime > 2000) {
    currentState = IDLE;
  }
}

// ========================
// Core Functions
// ========================

void advanceStep() {

  programStep++;
  
  if (programStep >= totalSteps || PROGRAMS[currentProgram][programStep] == -1) {
    celebrateCompletion();
    currentProgram = (currentProgram + 1) % TRAININGS;
    currentState = IDLE;
    return;
  }
  
  currentStep = PROGRAMS[currentProgram][programStep];
  currentState = BLINK_TARGET;
}

void setProgram(int programIndex) {
  // Validate program index
  currentProgram = constrain(programIndex, 0, TRAININGS-1);
  programStep = 0;
  currentStep = PROGRAMS[programIndex][0];
  
  totalSteps = 0;
  while (totalSteps < MAX_STEPS && PROGRAMS[programIndex][totalSteps] != -1) {
    totalSteps++;
  }
  
  resetAllLEDs();
  currentState = BLINK_TARGET;
}

void celebrateCompletion() {
  const int flashCount = 3;
  const int flashDuration = 300;
  
  for (int i = 0; i < flashCount; i++) {
    // Green flash
    for (int led = 0; led < MAX_LEDS; led++) {
      digitalWrite(ALL_LEDS[led], HIGH);
    }
    delay(flashDuration);
    resetAllLEDs();
    
    // Red flash
    for (int led = 0; led < MAX_LEDS; led++) {
      digitalWrite(ALL_LEDS[led + RED_OFFSET], HIGH);
    }
    delay(flashDuration);
    resetAllLEDs();
  }
}

void resetAllLEDs() {
  for (int i = 0; i < MAX_LEDS*2; i++) {
    digitalWrite(ALL_LEDS[i], LOW);
  }
}

// Enhanced error handling
void triggerError(States &errorSourceState) {
  errorStartTime = millis();
  resetAllLEDs();
  
  // Solid red for current step
  int errorLed = (currentStep % MAX_LEDS) + RED_OFFSET;
  setLedSolid(errorLed);
  
  currentState = ERROR;
  
  // State-specific recovery
  errorSourceState = (errorSourceState == VERIFY_NEXT) ? WAIT_FOR_HOLD : BLINK_TARGET;
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
    case ERROR:       handleError(currentTime); break;
  }

  
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