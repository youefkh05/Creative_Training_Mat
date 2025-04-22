// Pin Definitions
const int BUTTONS[] = {0, 1, 2, 3, A0};  // B1-B5 (HIGH when pressed)
const int GREEN_LEDS[] = {4, 5, 6, 7, 8}; // LG1-LG5
const int RED_LEDS[] = {9, 10, 11, 12, 13}; // LR1-LR5

// State Machine
enum States {
  BLINK_TARGET,
  WAIT_FOR_HOLD, 
  VERIFY_NEXT,
  ERROR
};

// ========================
// 1. Define Training Programs
// ========================
const int TRAININGS = 3; // Number of available programs
const int MAX_STEPS = 5;  // Maximum steps per program

// Exercise sequences for each program
const int PROGRAMS[TRAININGS][MAX_STEPS] = {
  {0, 1, 2, 3, 4},  // Program 0: Head → R Hand → L Hand → R Leg → L Leg (default)
  {0, 2, 1, 3, -1}, // Program 1: Head → L Hand → R Hand → R Leg (4 steps)
  {4, 3, 2, -1, -1}  // Program 2: L Leg → R Leg → L Hand (3 steps)
};

States currentState = BLINK_TARGET;
int currentProgram = 0;
int programStep = PROGRAMS[0][0];
int totalSteps = MAX_STEPS; // Will be adjusted per program
int nextStep = -1;
unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup() {
  // Initialize buttons as INPUT (not PULLUP)
  for (int i = 0; i < 5; i++) {
    pinMode(BUTTONS[i], INPUT); // Button returns HIGH when pressed
    pinMode(GREEN_LEDS[i], OUTPUT);
    pinMode(RED_LEDS[i], OUTPUT);
    digitalWrite(GREEN_LEDS[i], LOW);
    digitalWrite(RED_LEDS[i], LOW);
  }
}

// ========================
// loop() Section
// ========================

void loop() {
  unsigned long currentTime = millis();
  
  switch(currentState) {
    case BLINK_TARGET:
      // Blink current target LED (500ms interval)
      if (currentTime - lastBlinkTime >= 500) {
        ledState = !ledState;
        digitalWrite(GREEN_LEDS[currentStep], ledState);  // Use currentStep
        lastBlinkTime = currentTime;
      }
      
      // Transition when target button is pressed (HIGH)
      if (digitalRead(BUTTONS[currentStep]) == HIGH) {    // Use currentStep
        digitalWrite(GREEN_LEDS[currentStep], HIGH);       // Use currentStep
        currentState = WAIT_FOR_HOLD;
      }
      break;

    case WAIT_FOR_HOLD:
      // Get the next step FROM THE CURRENT PROGRAM
      if (programStep + 1 < totalSteps) {
        nextStep = PROGRAMS[currentProgram][programStep + 1];
      } else {
        nextStep = -1; // End of program
      }

      // Only proceed if there's a next step
      if (nextStep != -1) {
        // Solid current LED, blink next LED
        if (currentTime - lastBlinkTime >= 500) {
          ledState = !ledState;
          digitalWrite(GREEN_LEDS[nextStep], ledState);    // nextStep is correct here
          lastBlinkTime = currentTime;
        }

        // Verify user maintains pressure (still HIGH)
        if (digitalRead(BUTTONS[currentStep]) == LOW) {   // Use currentStep
          triggerError(currentState);
        } 
        else if (digitalRead(BUTTONS[nextStep]) == HIGH) {
          digitalWrite(GREEN_LEDS[nextStep], HIGH);
          currentState = VERIFY_NEXT;
        }
      }
      break;

    case VERIFY_NEXT:
      delay(500); // Debounce
      if (digitalRead(BUTTONS[currentStep]) == HIGH &&     // Use currentStep
          digitalRead(BUTTONS[nextStep]) == HIGH) {
        advanceStep();
      } else {
        triggerError(currentState);
      }
      break;

    case ERROR:
      // Error handling remains the same
      break;
  }
}

// ========================
// 2. Modified State Machine
// ========================
void advanceStep() {
  
  // Move to next position in program array
  programStep++;
  
  // Check if program completed
  if (programStep >= totalSteps || PROGRAMS[currentProgram][programStep] == -1) {
    celebrateCompletion();
    // Switch to next program
    currentProgram = (currentProgram + 1) % TRAININGS;
    setProgram(currentProgram);
    return;
  }
  
  // Update currentStep with the next step from program
  currentStep = PROGRAMS[currentProgram][programStep];
  currentState = BLINK_TARGET;
}

// ========================
// 3. Program Initialization
// ========================

void setProgram(int programIndex) {
  currentProgram = programIndex;
  programStep = 0; // Reset array position
  currentStep = PROGRAMS[programIndex][0]; // Set first physical step
  
  // Calculate actual steps
  totalSteps = 0;
  while (totalSteps < MAX_STEPS && PROGRAMS[programIndex][totalSteps] != -1) {
    totalSteps++;
  }
  
  resetAllLEDs();
  currentState = BLINK_TARGET;
}

// ========================
// 5. Helper Functions
// ========================
void celebrateCompletion() {
  // Flash all green LEDs 3 times
  for (int i = 0; i < 3; i++) {
    for (int led = 0; led < 5; led++) {
      digitalWrite(GREEN_LEDS[led], HIGH);
    }
    delay(300);
    for (int led = 0; led < 5; led++) {
      digitalWrite(GREEN_LEDS[led], LOW);
    }
    delay(300);
  }
}

void resetAllLEDs() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(GREEN_LEDS[i], LOW);
    digitalWrite(RED_LEDS[i], LOW);
  }
}

void triggerError( States &errorSourceState) {
  /* Blink current step's red LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_LEDS[programStep], HIGH);
    delay(200);
    digitalWrite(RED_LEDS[programStep], LOW);
    delay(200);
  }

  // State transition rules
  if (errorSourceState == VERIFY_NEXT) {
    errorSourceState = WAIT_FOR_HOLD;  // Step back one state
    digitalWrite(GREEN_LEDS[programStep], HIGH); // Keep current LED solid
  } 
  else {
    errorSourceState = BLINK_TARGET;  // Full reset for other errors
    digitalWrite(GREEN_LEDS[programStep], LOW); // Turn off current LED
  }
  */
  
  // Define where each state should go on error
  switch(errorSourceState) {
    case VERIFY_NEXT:
      errorSourceState = WAIT_FOR_HOLD;  // Your requested behavior
      break;
    case WAIT_FOR_HOLD:
    default:
      errorSourceState = BLINK_TARGET;   // Full reset for other cases
      break;
  }

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

*/