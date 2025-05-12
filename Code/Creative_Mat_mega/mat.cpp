#include "mat.h"

// Pin Definitions
const int BUTTONS[] = {2, 3, 4, 5, 6, 7};  // B1-B6 (LOW when pressed)
// 0 → LEFT_HAND, 1 → RIGHT_HAND, 2 → LEFT_KNEE, 3 → RIGHT_KNEE, 4 → LEFT_LEG 5 →RIGHT_LEG
  

const int ALL_LEDS[] = {9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}; // [0-5]=Green, [6-12]=Red

#define RED(i) ((i) + RED_OFFSET)
// Exercise sequences for each program
const int PROGRAMS[TRAININGS][MAX_STEPS] = {
  {RED(RIGHT_HAND), RED(LEFT_KNEE), RED(LEFT_LEG), RED(RIGHT_HAND), RED(LEFT_HAND), RED(RIGHT_KNEE), RIGHT_KNEE, LEFT_HAND, RIGHT_HAND, -1},  // Program 0: Cat Cow
  { RED(RIGHT_HAND), RIGHT_HAND, RED(RIGHT_HAND), RIGHT_HAND, RED(RIGHT_HAND), RIGHT_HAND, -1 },
  { RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, -1 },
  { RED(LEFT_KNEE), RED(LEFT_LEG), RED(RIGHT_HAND), LEFT_KNEE, RIGHT_HAND, LEFT_LEG, RIGHT_KNEE, RED(RIGHT_KNEE), -1 },
  { RED(LEFT_HAND), RED(RIGHT_HAND), RED(LEFT_KNEE), RED(RIGHT_KNEE), RED(LEFT_LEG), LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, LEFT_LEG, -1 },
  { RED(RIGHT_KNEE), RIGHT_KNEE, LEFT_HAND, RED(LEFT_HAND), RIGHT_KNEE, RED(RIGHT_KNEE), -1 },
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
unsigned long errorStartTime = 0;
bool buttonStates[MAX_LEDS] = {LOW}; // Track button states for debouncing
unsigned long blinkTimers[MAX_LEDS * 2] = {0};  // Track last blink time for each LED
bool ledStates[MAX_LEDS * 2] = {false};         // Track on/off state of each LED


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

  #ifdef DEBUG
    Serial.println("Init");
    delay(500);
  #endif
  
}

// ========================
// LED Control Functions
// ========================

void blinkLed(int step, unsigned long currentTime, bool oppos) {
  int index = step;
  int physicalPin = ALL_LEDS[index];

  if (currentTime - blinkTimers[index] >= BLINK_INTERVAL) {
    ledStates[index] = !ledStates[index];
    digitalWrite(physicalPin, ledStates[index]);
    
    #ifdef DEBUG
    Serial.print("Blink led:");
    Serial.println(index);
    delay(500);
    #endif
    

    // Turn off opposite color when needed
    if (oppos == HIGH) {
      int oppIndex = (step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET;
      digitalWrite(ALL_LEDS[oppIndex], LOW);
      ledStates[oppIndex] = false;
      
      #ifdef DEBUG
      Serial.print("blink off led:");
      Serial.println(oppIndex);
      delay(500);
      #endif
    }

    blinkTimers[index] = currentTime;
  }
}

void setLedSolid(int step, bool oppos) {
    int physicalPin = ALL_LEDS[step];
    ledStates[step] = HIGH;
    digitalWrite(physicalPin, HIGH);

    #ifdef DEBUG
    Serial.print("set on led:");
    Serial.println(step);
    delay(500);
    #endif
    

    // Turn off opposite color (When needed)
    if(oppos == HIGH){
    int oppIndex = (step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET;
    int oppositeColorPin = ALL_LEDS[oppIndex];

    #ifdef DEBUG
    Serial.print("set off led:");
    Serial.println(oppIndex);
    delay(500);
    #endif

    digitalWrite(oppositeColorPin, LOW);
    ledStates[oppIndex] = LOW;
    }

}

void resetLedSolid(int step, bool oppos) {
    int physicalPin = ALL_LEDS[step];
    ledStates[step] = LOW;
    digitalWrite(physicalPin, LOW);

    #ifdef DEBUG
    Serial.print("reset off led:");
    Serial.println(step);
    delay(500);
    #endif

    // Turn on opposite color (When needed)
    if(oppos == HIGH){
      int oppIndex = (step >= RED_OFFSET) ? step - RED_OFFSET : step + RED_OFFSET;
      int oppositeColorPin = ALL_LEDS[oppIndex];
      digitalWrite(oppositeColorPin, HIGH);
      ledStates[oppIndex] = HIGH;
      #ifdef DEBUG
      Serial.print("reset on led:");
      Serial.println(oppIndex);
      delay(500);
      #endif
    }

}

void resetAllLEDs() {
  ledState = false;

  #ifdef DEBUG
      Serial.println("reset all leds:");
      delay(500);
  #endif

  for (int i = 0; i < MAX_LEDS*2; i++) {
    ledStates[i] = LOW;
    digitalWrite(ALL_LEDS[i], LOW);
  }
}

// Improved button reading with debounce
bool isButtonPressed(int buttonIndex) {
  static unsigned long lastDebounceTime[MAX_LEDS] = {0};
  static bool lastButtonState[MAX_LEDS] = {HIGH};

  #ifdef DEBUG
      Serial.println("button pressed");
      delay(500);
  #endif

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

  #ifdef DEBUG
      Serial.print("Trigger Error state:");
      Serial.println(errorSourceState);
      delay(500);
  #endif
}

void handleIdle() {

  #ifdef DEBUG
      Serial.println("Handle idle");
      delay(500);
  #endif

  if (digitalRead(BUTTONS[0]) == LOW) { // Start program on button 0 press
    setProgram(currentProgram);
  }
}

void handleBlinkTarget(unsigned long currentTime) {

  #ifdef DEBUG
      Serial.println("Blink Target");
      delay(500);
  #endif

  blinkLed(currentStep, currentTime, LOW); // Always turn off opposite LED
  
  // RED LED (release): wait for button to be LOW
  if (currentStep >= RED_OFFSET) {
    if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == HIGH) {
      #ifdef DEBUG
        Serial.println("Blink Target red");
        delay(500);
      #endif
      setLedSolid(currentStep, LOW); 
      currentState = WAIT_FOR_HOLD;
    }
  } 
  // GREEN LED (press): wait for button to be HIGH
  else if (digitalRead(BUTTONS[currentStep % MAX_LEDS]) == LOW) {
    #ifdef DEBUG
        Serial.println("Blink Target green");
        delay(500);
    #endif
    setLedSolid(currentStep, LOW); 
    currentState = WAIT_FOR_HOLD;
  }
}

void handleWaitForHold(unsigned long currentTime) {
  #ifdef DEBUG
        Serial.println("handle Wait");
        delay(500);
  #endif

  if (programStep + 1 < totalSteps) {
    nextStep = PROGRAMS[currentProgram][programStep + 1];
    if(programStep>0){
        preStep = PROGRAMS[currentProgram][programStep - 1];
    }

  } else {
    nextStep = -1;
  }

  #ifdef DEBUG
        Serial.print("handle Wait next");
        Serial.println(nextStep);
        delay(500);
  #endif  

  if (nextStep != -1) {
    // Handle special same-button transition (green↔red)
    if (abs(nextStep - currentStep) == RED_OFFSET) {

      #ifdef DEBUG
        Serial.print("Special same button current:");
        Serial.print(currentStep);
        Serial.print("Special same button next:");
        Serial.println(nextStep);
        delay(500);
      #endif  
      
      blinkLed(nextStep, currentTime, HIGH);
      
      // Wait for button state to match next step's requirement
      bool nextButtonReady = (nextStep >= RED_OFFSET) 
        ? digitalRead(BUTTONS[nextStep % MAX_LEDS])  // Red: button released
        : !digitalRead(BUTTONS[nextStep % MAX_LEDS]);  // Green: button pressed

      if (nextButtonReady) {
        programStep++;
        currentStep = nextStep;
        currentState = BLINK_TARGET;  // Restart cycle for the new step

         #ifdef DEBUG
          Serial.print("Finished Special same button current:");
          Serial.print(currentStep);
          Serial.print("Special same button next:");
          Serial.println(nextStep);
          delay(500);
         #endif  
      }
      return;
    }

    // Normal case (different buttons)
    #ifdef DEBUG
          Serial.print("Normal button current:");
          Serial.print(currentStep);
          Serial.print("Normal button next:");
          Serial.println(nextStep);
          delay(500);
    #endif  
    
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
      #ifdef DEBUG
          Serial.print("Finished Normal button current:");
          Serial.print(currentStep);
          Serial.print("Special Normal next:");
          Serial.println(nextStep);
          delay(500);
      #endif  
    }
  } else {
    advanceStep();
  }
}

void handleVerifyNext() {
  delay(HOLD_DURATION);
  #ifdef DEBUG
    Serial.println("Verify next");
  #endif  

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


void mat_checkerror() {
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

      // If the same button is involved in a later step, update expected state and position
      if (nextButtonIndex == buttonIndex) {
        expectedState = (nextStep >= RED_OFFSET) ? HIGH : LOW;
        position = j;
      }
    }

    // If actual state does not match expected
    if (actualState != expectedState) {
      int nextindex = currentStep % MAX_LEDS;
      int futureindex = nextStep % MAX_LEDS;
      // 🚫 FIX 1: Only trigger error if current step is unrelated to this button
      if ((nextindex != buttonIndex) && (futureindex != buttonIndex)) {

        // ✅ FIX 2: Before resetting, check if the user has already corrected the issue
        if (digitalRead(BUTTONS[buttonIndex]) != expectedState) {
          // Still an error, trigger error handling
          programStep = position;
          currentStep = step;
          currentState = BLINK_TARGET;
          resetAllLEDs();
          #ifdef DEBUG

            Serial.print("mat check error nextindex:");
            Serial.println(nextindex);
            Serial.print("futureindex:");
            Serial.println(futureindex);
            Serial.print("buttonIndex:");
            Serial.println(buttonIndex);
            Serial.print("program step:");
            Serial.print(programStep);
            Serial.print("currentStep:");
            Serial.print(step);
            delay(500);
          #endif  
          return;
        }

        // 🟢 If user already fixed it, skip to next
        continue;
      }
    }
  }
}

// ========================
// Core Functions
// ========================

void advanceStep() {

  programStep++;

  #ifdef DEBUG
    Serial.print("Advance Step prog step:");
    Serial.println(programStep);
  #endif

  if (programStep >= totalSteps || PROGRAMS[currentProgram][programStep] == -1) {
    celebrateCompletion();
    currentProgram = (currentProgram + 1) % TRAININGS;
    currentState = IDLE;

    #ifdef DEBUG
      Serial.print("Finished program start program:");
      Serial.println(currentProgram);
    #endif

    return;
  }

  currentStep = PROGRAMS[currentProgram][programStep];

  currentState = BLINK_TARGET;

  #ifdef DEBUG
      Serial.print("currentStep");
      Serial.println(currentStep);
  #endif

}

void setProgram(int programIndex) {
  currentState = IDLE;     // Start in idle
  preStep = -1;
  nextStep = -1;

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

  #ifdef DEBUG
    Serial.print("Set program currentProgram");
    Serial.println(currentProgram);
    Serial.print("Total steps");
    Serial.println(totalSteps);
  #endif
}

void celebrateCompletion() {
  const int flashCount = 3;
  const int flashDuration = 300;
  
  for (int i = 0; i < flashCount; i++) {
    // Green flash
    for (int led = 0; led < MAX_LEDS; led++) {
      ledStates[led] = HIGH;
      digitalWrite(ALL_LEDS[led], HIGH);
    }
    delay(flashDuration);
    resetAllLEDs();
    
    // Red flash
    for (int led = 0; led < MAX_LEDS; led++) {
       ledStates[led + RED_OFFSET] = HIGH;
      digitalWrite(ALL_LEDS[led + RED_OFFSET], HIGH);
    }
    delay(flashDuration);
    resetAllLEDs();
  }
}




