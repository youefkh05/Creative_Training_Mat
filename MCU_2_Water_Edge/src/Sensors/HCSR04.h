/*************************************************************************
 * 
 *  Author:     Ameer Louly
 * 
 *  Date:       02/26/2025
 * 
*************************************************************************/

#ifndef HCSR04_H
#define HCSR04_H

#include "../std_types.h"
#include <Arduino.h>

#define TRIG_PIN 9
#define ECHO_PIN 10

void HCSR04_init(void);
// Intializes the Sensors

void sendTrig(void);
// Sends Appropriate trigger to Ultrasonic HCSR04

float32 getDistanceinCM_HCSR04(void);
// Calculates the returns the distance measured by the sensor

#endif