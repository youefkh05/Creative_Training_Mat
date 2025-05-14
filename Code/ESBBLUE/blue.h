/**
 *      Author: Yousef
 *      Date: 5/14/2025
 */

#ifndef BLUE_H
#define BLUE_H

#include "BluetoothSerial.h"
#include "mat.h"

/* BLUE TEXT START */
#define BLUE_NAME "Creative-MAT"
#define MAT_LINK "https://drive.google.com/drive/folders/1YBFMJKyItKko7F_0Hsu1N62fj0dlx9bX?usp=sharing"
#define BLUE_WELCOME "Welcome to creative map!\n hope you have fun :)"
#define MAT_EXTRA_MES "🎁 Extra preview done! More exciting stuff coming your way! 😉"

extern const char* driveLinks[];
extern const char* training_messages[];
extern const char* program_finish_messages[];
/* BLUE TEXT END */

/* global variables*/
extern BluetoothSerial Bluetooth_serialESP;

/* global variables end*/



void BLUE_init(void);

void BLUE_Start(void);

void BLUE_Problem_intro(int problemidx);

void BLUE_Problem_outro(int problemidx);

void BLUE_MAT(bool program_flag);

#endif