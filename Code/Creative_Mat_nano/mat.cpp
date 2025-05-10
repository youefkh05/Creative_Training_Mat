#include "mat.h"


// Pin Definitions
const int BUTTONS[] = {0, 1, 2, 3, A0};  // B1-B5 (HIGH when pressed)
// 0 → Head, 1 → Right Hand, 2 → Left Hnad, 3 → Right Leg, 4 → Left Leg

const int ALL_LEDS[] = {4,5,6,7,8, 9,10,11,12,13}; // [0-4]=Green, [5-9]=Red

#define RED(i) ((i) + RED_OFFSET)
// Exercise sequences for each program
const int PROGRAMS[TRAININGS][MAX_STEPS] = {
  {RED(LEFT_HAND), RED(RIGHT_HAND), RED(LEFT_KNEE),RED(RIGHT_KNEE), RED(LEFT_LEG), RED(LEFT_HAND), RED(RIGHT_HAND), RED(LEFT_KNEE),RED(RIGHT_KNEE), RED(LEFT_LEG)},  // Program 0: Cat Cow
  {2, 2, 3, 4, RED(1), 1, RED(4), 4, RED(1), RED(4)},  // Program 0: Cat Cow
  {1, 2+RED_OFFSET, 1, 3, -1},  // Program 1: Step 2 uses red
  {4+RED_OFFSET, 3, 2, -1, -1}  // Program 2: Step 0 uses red
};

// Global Variables
States currentState = IDLE;     // Start in idle
int currentProgram = 0;
int programStep = 0;
int totalSteps = MAX_STEPS;     // Will be adjusted per program
int preStep = -1;
int currentStep = PROGRAMS[currentProgram][0];
int nextStep = -1;
unsigned long lastBlinkTime = 0;
bool ledState = false;
bool check_error = false;
unsigned long errorStartTime = 0;
bool buttonStates[MAX_LEDS] = {LOW}; // Track button states for debouncing

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

void mat_init(){
    // Initialize all LEDs and buttons
  for (int i = 0; i < MAX_LEDS; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(ALL_LEDS[i], OUTPUT);          // Green LEDs
    pinMode(ALL_LEDS[i + RED_OFFSET], OUTPUT); // Red LEDs
    digitalWrite(ALL_LEDS[i], LOW);
    digitalWrite(ALL_LEDS[i + RED_OFFSET], LOW);
  }
}

// ========================
// LED Control Functions
// ========================

void blinkLed(int step, unsigned long currentTime, bool oppos) {
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      ledState = !ledState;
      int physicalPin = ALL_LEDS[step];
      digitalWrite(physicalPin, ledState);
      
      // Turn off opposite color (When needed)
      if(oppos == HIGH){
      int oppositeColorPin = ALL_LEDS[(step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET];
      digitalWrite(oppositeColorPin, LOW);
      }
      
      lastBlinkTime = currentTime;
    }
}

void setLedSolid(int step, bool oppos) {
    int physicalPin = ALL_LEDS[step];
    digitalWrite(physicalPin, HIGH);

    // Turn off opposite color (When needed)
    if(oppos == HIGH){
    int oppositeColorPin = ALL_LEDS[(step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET];
    digitalWrite(oppositeColorPin, LOW);
    }

}

void resetLedSolid(int step, bool oppos) {
    int physicalPin = ALL_LEDS[step];
    digitalWrite(physicalPin, LOW);

    // Turn on opposite color (When needed)
    if(oppos == HIGH){
    int oppositeColorPin = ALL_LEDS[(step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET];
    digitalWrite(oppositeColorPin, HIGH);
    }

}

void resetAllLEDs() {
  ledState = false;
  for (int i = 0; i < MAX_LEDS*2; i++) {
    digitalWrite(ALL_LEDS[i], LOW);
  }
}

// Improved button reading with debounce
bool isButtonPressed(int buttonIndex) {
  static unsigned long lastDebounceTime[MAX_LEDS] = {0};
  static bool lastButtonState[MAX_LEDS] = {HIGH};
  
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

// Enhanced error handling
void triggerError(States &errorSourceState) {
  errorStartTime = millis();
  
  // State-specific recovery
  errorSourceState = (errorSourceState == VERIFY_NEXT) ? WAIT_FOR_HOLD : BLINK_TARGET;
}

void handleIdle() {
  if (digitalRead(BUTTONS[0]) == LOW) { // Start program on button 0 press
    setProgram(currentProgram);
  }
}

void handleBlinkTarget(unsigned long currentTime) {
  blinkLed(currentStep, currentTime, LOW); // Always turn off opposite LED
  
  // RED LED (release): wait for button to be LOW
  if (currentStep >= RED_OFFSET) {
    if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == HIGH) {
      setLedSolid(currentStep, LOW); 
      currentState = WAIT_FOR_HOLD;
    }
  } 
  // GREEN LED (press): wait for button to be HIGH
  else if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == LOW) {
    setLedSolid(currentStep, LOW); 
    currentState = WAIT_FOR_HOLD;
  }
}

void handleWaitForHold(unsigned long currentTime) {
  if (programStep + 1 < totalSteps) {
    nextStep = PROGRAMS[currentProgram][programStep + 1];
    if(programStep>0)
        preStep = PROGRAMS[currentProgram][programStep - 1];
  } else {
    nextStep = -1;
  }

  if (nextStep != -1) {
    // Handle special same-button transition (green↔red)
    if (abs(nextStep - currentStep) == RED_OFFSET) {
      
      blinkLed(nextStep, currentTime, HIGH);
      
      // Wait for button state to match next step's requirement
      bool nextButtonReady = (nextStep >= RED_OFFSET) 
        ? digitalRead(BUTTONS[nextStep % MAX_LEDS])  // Red: button released
        : !digitalRead(BUTTONS[nextStep % MAX_LEDS]);  // Green: button pressed

      if (nextButtonReady) {
        programStep++;
        currentStep = nextStep;
        currentState = BLINK_TARGET;  // Restart cycle for the new step
      }
      return;
    }

    // Normal case (different buttons)
    setLedSolid(currentStep, HIGH);
    blinkLed(nextStep, currentTime, HIGH);

    // Check current button state validity
    bool currentButtonValid = (currentStep >= RED_OFFSET) 
      ? digitalRead(BUTTONS[currentStep % MAX_LEDS])  // Red: released
      : !digitalRead(BUTTONS[currentStep % MAX_LEDS]);  // Green: pressed

    if (!currentButtonValid) {
      triggerError(currentState);
      return;
    }

    // Check next button initiation
    bool nextButtonInitiated = (nextStep >= RED_OFFSET)
      ? digitalRead(BUTTONS[nextStep % MAX_LEDS])  // Red: released
      : !digitalRead(BUTTONS[nextStep % MAX_LEDS]);  // Green: pressed

    if (nextButtonInitiated) {
      programStep++;
      currentStep = nextStep;
      currentState = VERIFY_NEXT;
    }
  } else {
    advanceStep();
  }
}

void handleVerifyNext(bool &check) {
  delay(HOLD_DURATION);
  check = true;
  // Verify next button is still in correct state
  bool nextButtonOk = (nextStep >= RED_OFFSET)
    ? (digitalRead(BUTTONS[nextStep % MAX_LEDS]) == HIGH)
    : (digitalRead(BUTTONS[nextStep % MAX_LEDS]) == LOW);

  if (nextButtonOk) {
    advanceStep();
  } else {
    triggerError(currentState);
  }
}

void handleError(unsigned long currentTime) {
  
  if (millis() - errorStartTime > ERROR_DURATION) {
    currentState = IDLE;
  }
}

void mat_checkerror(bool &check) {
    if(check==true){
        for (int i = 0; i < programStep; ++i) {
            int step = PROGRAMS[currentProgram][i];
            int position = i;
            int buttonIndex = step % MAX_LEDS;
            bool expectedState = (step >= RED_OFFSET) ? HIGH : LOW; // Red = released, Green = pressed
            bool actualState = digitalRead(BUTTONS[buttonIndex]);
            resetLedSolid(step, LOW);

            // Loop through upcoming steps to check if the same button is pressed/released
            for (int j = i + 1; j < programStep; ++j) {
            int nextStep = PROGRAMS[currentProgram][j];
            int nextButtonIndex = nextStep % MAX_LEDS;
            
            // If the same button is involved and the next step is either pressed or released
            if (nextButtonIndex == buttonIndex) {
                expectedState = (nextStep >= RED_OFFSET) ? HIGH : LOW;
                position = j;
            }
            }

            // If the current state does not match the expected state, raise an error
            if (actualState != expectedState) {
            int nextindex = currentStep %MAX_LEDS;
            if(nextindex != buttonIndex){  
                programStep = position;
                currentStep = step;
                currentState = BLINK_TARGET;
                resetAllLEDs();
                return;
            }
            }
        }
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
    check_error = false;
    return;
  }

  currentStep = PROGRAMS[currentProgram][programStep];
  currentState = BLINK_TARGET;
}

void setProgram(int programIndex) {
    currentState = IDLE;     // Start in idle
    preStep = -1;
    nextStep = -1;
    check_error = false;
    
    // Validate program index
    currentProgram = constrain(programIndex, 0, TRAININGS-1);
    programStep = 0;
    currentStep = PROGRAMS[currentProgram][0];
    
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




