/**
 *      Author: Yousef Khaled
 *      Date: 5/10/2025
 */

#ifndef MAT_H
#define MAT_H

#include <Arduino.h>

// ==== Enum for State Machine ====
enum States {
  IDLE,
  BLINK_TARGET,
  WAIT_FOR_HOLD,
  VERIFY_NEXT,
  ERROR
};

enum BodyPart {
  LEFT_HAND =0,
  RIGHT_HAND,
  LEFT_KNEE,
  RIGHT_KNEE,
  LEFT_LEG,
  RIGHT_LEG
};


// ========================
// 1. Define Training Programs
// ========================
#define MAX_STEPS           13   // Maximum steps per program
#define PROBLEMS            9   // Maximum Number of problems
#define TrainingPerProblem  4    // Maximum Number of problems
#define MAX_LEDS            6    // Number of physical LEDs per color
#define RED_OFFSET          MAX_LEDS  // Red LEDs start at index MAX_LEDS
//#define DEBUG 

// Add debounce constants
#define DEBOUNCE_DELAY  50
#define HOLD_DURATION   500
#define BLINK_INTERVAL  500
#define ERROR_DURATION  2000

// ==== Pin Arrays ====
extern const int BUTTONS[];
extern const int ALL_LEDS[];

// ==== Program Steps ====
extern const char training_problem[PROBLEMS][26];
extern const char problem_program[TrainingPerProblem*PROBLEMS][26];
extern const char body_part[MAX_LEDS][11];
extern const int PROGRAMS[TrainingPerProblem*PROBLEMS][MAX_STEPS];
extern const int HOLD[TrainingPerProblem*PROBLEMS][MAX_STEPS];

// ==== Global State Variables ====
extern States currentState;
extern int currentProgram;
extern int programStep;
extern int totalSteps;
extern int preStep;
extern int currentStep;
extern int nextStep;
extern int oled_step;
extern int specialidx;
extern unsigned long lastBlinkTime;
extern bool ledState;
extern unsigned long errorStartTime;
extern bool buttonStates[MAX_LEDS];
extern bool hold_flag;

// ==== Functions ====
#ifdef DEBUG
void printState();
#endif

void mat_init();
void blinkLed(int step, unsigned long currentTime, bool oppos);
void setLedSolid(int step, bool oppos);
void resetLedSolid(int step, bool oppos);
void resetAllLEDs();
bool isButtonPressed(int buttonIndex);

void handleIdle();
void handleBlinkTarget(unsigned long currentTime);
void handleWaitForHold(unsigned long currentTime);
void handleVerifyNext();
void handleError(unsigned long currentTime);
void mat_checkerror();

void advanceStep();
void setProgram(int programIndex, bool start);
void triggerError(States &errorSourceState);
void celebrateCompletion();

void mat_loop();

#endif // MAT_H
