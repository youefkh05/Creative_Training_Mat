#include "mat.h"

// Pin Definitions
const int BUTTONS[] = {2, 3, 4, 5, 12, 13};  // B1-B6 (LOW when pressed)
// 0 → LEFT_HAND, 1 → RIGHT_HAND, 2 → LEFT_KNEE, 3 → RIGHT_KNEE, 4 → LEFT_LEG 5 →RIGHT_LEG
  

const int ALL_LEDS[] = {14, 15, 16, 17,18, 19, 23, 25, 26, 27, 32, 33}; // [0-5]=Green, [6-12]=Red

const char training_problem[PROBLEMS][26]={
  {"Core exercises"},
  {"Knee pain"},
  {"Kyphosis"},
  {"Lumbar flat"},
  {"low back pain"},
  {"pirifrms strch"},
  {"plank"},
  {"push ups"},
  {"Extra"},
};

const char problem_program[TrainingPerProblem*PROBLEMS][26]={
  //problem 0
  {"Bear plank"},
  {"Bird-Dog"},
  {"Mountin climbr"},
  {"Shoulder taps"},

  //problem 1
  {"Heel propL"},
  {"Heel propR"},
  {"Quad sets"},
  {"Extra1"},

  //problem 2
  {"Bird-Dog"},
  {"Cat cow"},
  {"Scapular cat"},
  {"Thread needle"},

  //problem 3
  {"Bird-Dog"},
  {"Dead Bug"},
  {"Glute Bridge"},
  {"Pelvic Tilt"},

  //problem 4
  {"Bird-Dog"},
  {"Cat cow"},
  {"Child Pose"},
  {"Extra1"},

  //problem 5
  {"pirifrms strch"},
  {"Extra1"},
  {"Extra2"},
  {"Extra3"},

  //problem 6
  {"Plank L"},
  {"Plank R"},
  {"Extra2"},
  {"Extra3"},

  //problem 7
  {"Push-Up"},
  {"Extra1"},
  {"Extra2"},
  {"Extra3"},

  //problem 8
  {"Extra1"},
  {"Extra2"},
  {"Extra3"},
  {"Extra4"},
};

const char body_part[MAX_LEDS][11]={
  {"LEFT HAND"},
  {"RIGHT HAND"},
  {"LEFT KNEE"},
  {"RIGHT KNEE"},
  {"LEFT LEG"},
  {"RIGHT LEG"},
};

#define RED(i) ((i) + RED_OFFSET)
// Exercise sequences for each program
const int PROGRAMS[TrainingPerProblem*PROBLEMS][MAX_STEPS] = {
  //problem 0
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, LEFT_LEG, RIGHT_LEG, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_KNEE, RIGHT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, RED(RIGHT_HAND), RED(LEFT_KNEE), RIGHT_HAND, LEFT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_LEG, RIGHT_LEG, RED(RIGHT_LEG),RIGHT_LEG, RED(LEFT_LEG),LEFT_LEG,-1},  
  { LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG, RED(RIGHT_HAND),RIGHT_HAND, RED(LEFT_HAND), LEFT_HAND, -1 },
  
  //problem 1
  { LEFT_HAND, RIGHT_HAND, LEFT_LEG, RED(LEFT_KNEE), LEFT_LEG, LEFT_KNEE, RED(LEFT_LEG), LEFT_LEG, LEFT_KNEE, RED(LEFT_KNEE), LEFT_KNEE, -1},
  { LEFT_HAND, RIGHT_HAND, RIGHT_LEG, RED(RIGHT_KNEE), RIGHT_LEG, RIGHT_KNEE, RED(RIGHT_LEG), RIGHT_LEG, RIGHT_KNEE, RED(RIGHT_KNEE), RIGHT_KNEE, -1},
  { LEFT_HAND, RIGHT_HAND, LEFT_LEG, RIGHT_LEG, RED(RIGHT_LEG), RED(RIGHT_LEG), RED(LEFT_LEG), RED(LEFT_LEG),-1 },
  {-1},
  
  //problem 2
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, RED(RIGHT_HAND), RED(LEFT_KNEE), RIGHT_HAND, LEFT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, LEFT_LEG, RIGHT_LEG, RED(LEFT_HAND), RED(RIGHT_HAND), -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(RIGHT_HAND), RIGHT_HAND, RED(LEFT_HAND), LEFT_HAND, RED(LEFT_HAND), -1 },
  
  //problem 3
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, RED(RIGHT_HAND), RED(LEFT_KNEE), RIGHT_HAND, LEFT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), RED(RIGHT_HAND), RED(LEFT_KNEE),  -1 },
  { LEFT_KNEE, RIGHT_KNEE, RED(RIGHT_KNEE), RED(LEFT_KNEE), LEFT_KNEE, RIGHT_KNEE, -1},
  { LEFT_LEG, RIGHT_LEG, LEFT_KNEE, RIGHT_KNEE, RED(RIGHT_LEG), RIGHT_LEG, RED(LEFT_LEG), LEFT_LEG,  -1 },
  
  //problem 4
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_HAND, RIGHT_KNEE, RED(RIGHT_HAND), RED(LEFT_KNEE), RIGHT_HAND, LEFT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, -1 },
  { LEFT_HAND, RIGHT_HAND, LEFT_KNEE, RIGHT_KNEE, RED(LEFT_HAND), RED(RIGHT_KNEE), LEFT_KNEE, RIGHT_KNEE, -1 },
  {-1},
  
  //problem 5
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG, RED(LEFT_LEG), RED(LEFT_KNEE), LEFT_LEG, LEFT_KNEE,-1},
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG, RED(RIGHT_LEG), RED(RIGHT_KNEE), RIGHT_LEG, RIGHT_KNEE,-1},
  {-1},
  {-1},
  
  //problem 6
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG, RED(LEFT_HAND), RED(RIGHT_HAND), RED(LEFT_KNEE), RED(RIGHT_KNEE), RED(LEFT_LEG), RED(RIGHT_LEG),-1},
  {-1},
  {-1},
  {-1},

  //problem 7
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG,-1},
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), -1},
  {LEFT_HAND, RIGHT_HAND,-1},
  {LEFT_HAND, RIGHT_HAND, RED(LEFT_KNEE), RED(RIGHT_KNEE), LEFT_LEG, RIGHT_LEG,-1},

  //problem 8
  {-1},
  {-1},
  {-1},
  {-1},
};

const int HOLD[TrainingPerProblem*PROBLEMS][MAX_STEPS] = {
  //problem 0
  { 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0},
  { 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 0, 3, 0, 3, 0,0},  
  { 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0 },
  
  //problem 1
  { 0, 0, 0, 0, 0, 30, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 0, 0, 30, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 0, 7, 2, 7, 2, 0},
  {0},
  
  //problem 2
  { 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 7, 0, 0, 0, 7, 0},
  { 0, 0, 0, 0, 0, 0, 0, 5, 0},
  { 0, 0, 0, 0, 5, 0, 5, 0, 5, 0},
  
  //problem 3
  { 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 0, 0, 5, 0, 5, 0},
  { 0, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 0, 5, 0, 5, 0,  0},
  
  //problem 4
  { 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0},
  { 0, 0, 0, 7, 0, 0, 0, 7, 0},
  { 0, 0, 0, 0, 0, 0, 0, 30, 0 },
  {0},
  
  //problem 5
  {0, 0, 0, 0, 0, 0, 0, 25, 0, 0,-1},
  {0, 0, 0, 0, 0, 0, 0, 25, 0, 0,-1},
  {0},
  {0},
  
  //problem 6
  {0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0},
  {0},
  {0},
  {0},
  
  //problem 7
  {0, 0, 0, 0, 0, 5, 0},
  {0, 0, 0, -1},
  {0, 0, 0},
  {0, 5, 0, 0, 0, 0, 0},

  //problem 8
  {0},
  {0},
  {0},
  {0},
};

// Global Variables
States currentState = IDLE;     // Start in idle
int currentProgram = 0;
int programStep = 0;
int totalSteps = MAX_STEPS;     // Will be adjusted per program
int preStep = -1;
int currentStep = PROGRAMS[currentProgram][0];
int nextStep = -1;
int oled_step = 0;
int specialidx = -1;
bool buttonStates[MAX_LEDS] = {LOW}; // Track button states for debouncing
unsigned long blinkTimers[MAX_LEDS * 2] = {0};  // Track last blink time for each LED
bool ledStates[MAX_LEDS * 2] = {false};         // Track on/off state of each LED
bool hold_flag = false;


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
  Serial.print(" | Pre: ");
  Serial.print(preStep);
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
    digitalWrite(ALL_LEDS[i], HIGH);
    delay(500);
    digitalWrite(ALL_LEDS[i], LOW);
    digitalWrite(ALL_LEDS[i + RED_OFFSET], HIGH);
    delay(500);
    digitalWrite(ALL_LEDS[i + RED_OFFSET], LOW);
  }

  #ifdef DEBUG
    Serial.begin(9600);
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
  oled_step = step;
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
  //errorStartTime = millis();
  
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

  setProgram(currentProgram, false);
  resetAllLEDs();
  
}

void handleBlinkTarget(unsigned long currentTime) {
  hold_flag = false;
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
      if(HOLD[currentProgram][programStep]>0){
        hold_flag = true; 
      }
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
    if(HOLD[currentProgram][programStep]>0){
        hold_flag = true; 
      } 
    currentState = WAIT_FOR_HOLD;
  }
}

void handleWaitForHold(unsigned long currentTime) {


  #ifdef DEBUG
        Serial.println("handle Wait");
        delay(500);
        Serial.print("HOLD: ");
        Serial.println(HOLD[currentProgram][programStep]);
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
     
      specialidx = programStep; 
      
      #ifdef DEBUG
        Serial.print("Special same button current:");
        Serial.print(currentStep);
        Serial.print("Special same button next:");
        Serial.println(nextStep);
        delay(500);
      #endif  

      delay(1000*HOLD[currentProgram][programStep]);
      
      blinkLed(nextStep, currentTime, HIGH);
      
      

      // Wait for button state to match next step's requirement
      bool nextButtonReady = (nextStep >= RED_OFFSET) 
        ? digitalRead(BUTTONS[nextStep % MAX_LEDS])  // Red: button released
        : !digitalRead(BUTTONS[nextStep % MAX_LEDS]);  // Green: button pressed

      if (nextButtonReady) {
        programStep++;
        preStep = currentStep;
        currentStep = nextStep;
        currentState = BLINK_TARGET;  // Restart cycle for the new step
        hold_flag = false;
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
    delay(1000*HOLD[currentProgram][programStep]);
    // Check current button state validity
    bool currentButtonValid = (currentStep >= RED_OFFSET) 
      ? digitalRead(BUTTONS[currentStep % MAX_LEDS])  // Red: released
      : !digitalRead(BUTTONS[currentStep % MAX_LEDS]);  // Green: pressed
    
    blinkLed(nextStep, currentTime, HIGH);

    if (!currentButtonValid) {
      triggerError(currentState);
      return;
    }

    // Check next button initiation
    bool nextButtonInitiated = (nextStep >= RED_OFFSET)
      ? digitalRead(BUTTONS[nextStep % MAX_LEDS])  // Red: released
      : !digitalRead(BUTTONS[nextStep % MAX_LEDS]);  // Green: pressed

    if (nextButtonInitiated) {
      /*
      programStep++;
      preStep = currentStep;
      currentStep = nextStep;
      */ 
      hold_flag = false;
      currentState = VERIFY_NEXT;
      #ifdef DEBUG
          Serial.print("Finished Normal button current:");
          Serial.println(currentStep);
          Serial.print("Normal next:");
          Serial.println(nextStep);
          delay(500);
      #endif  
    }
  } else {
    advanceStep();
  }
}

void handleVerifyNext() {
  hold_flag = false;
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
  if(programStep<(totalSteps-1) && programStep>2){
    for (int i = (programStep-3); i < programStep; ++i){
      //ignore special case
      if( (i==specialidx) || (i==(specialidx+1)) ){
        continue;
      }

      int step = PROGRAMS[currentProgram][i];
      int position = i;
      int buttonIndex = step % MAX_LEDS;
      bool expectedState = (step >= RED_OFFSET) ? HIGH : LOW; // Red = released, Green = pressed
      bool actualState = digitalRead(BUTTONS[buttonIndex]);
      #ifdef DEBUG
        Serial.println("mat check error off:");
      #endif 
      
      // don't reset if it's the current led
      if(((currentStep % MAX_LEDS) != buttonIndex) &&  ((nextStep % MAX_LEDS) != buttonIndex) && ((preStep % MAX_LEDS) != buttonIndex) ){
        resetLedSolid(step, LOW);
      }
      // Loop through upcoming steps to check if the same button is pressed/released
      for (int j = i + 1; j < programStep; ++j) {
        int nextStepe = PROGRAMS[currentProgram][j];
        int nextButtonIndex = nextStepe % MAX_LEDS;

        // If the same button is involved in a later step, update expected state and position
        if (nextButtonIndex == buttonIndex) {
          expectedState = (nextStepe >= RED_OFFSET) ? HIGH : LOW;
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
            currentStep = PROGRAMS[currentProgram][programStep];
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
              Serial.println(programStep);
              Serial.print("currentStep:");
              Serial.println(step);
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
    currentState = IDLE;     // Start in idle
    programStep = 0;
    totalSteps = MAX_STEPS;     // Will be adjusted per program
    preStep = -1;
    currentStep = PROGRAMS[currentProgram][0];
    nextStep = -1;
    specialidx = -1;

    #ifdef DEBUG
      Serial.print("Finished program start program:");
      Serial.println(currentProgram);
    #endif

    return;
  }

  preStep = currentStep;
  currentStep = PROGRAMS[currentProgram][programStep];

  currentState = BLINK_TARGET;

  #ifdef DEBUG
      Serial.print("currentStep");
      Serial.println(currentStep);
  #endif

}

void setProgram(int programIndex, bool start) {
  currentState = IDLE;     // Start in idle
  preStep = -1;
  nextStep = -1;
  specialidx = -1;

  // Validate program index
  currentProgram = constrain(programIndex, 0, (TrainingPerProblem*PROBLEMS)-1);
  programStep = 0;
  currentStep = PROGRAMS[currentProgram][0];
  
  totalSteps = 0;
  while (totalSteps < MAX_STEPS && PROGRAMS[programIndex][totalSteps] != -1) {
    totalSteps++;
  }
  
  if(totalSteps !=0){
    resetAllLEDs(); 
  }
  else{
    currentState = IDLE;
  }

  if(start == true){
    currentState = BLINK_TARGET;
  }
  else{
    currentState = IDLE;     // Start in idle
  }

  if(PROGRAMS[currentProgram][0] == -1){
    currentState = IDLE;     // Start in idle
  }

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

void mat_loop(){

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


