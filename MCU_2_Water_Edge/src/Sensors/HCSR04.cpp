/*************************************************************************
 * 
 *  Author:     Ameer Louly
 * 
 *  Date:       02/26/2025
 * 
*************************************************************************/
#include "HCSR04.h"

void HCSR04_init(void)
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

/*****************************************************************************************************/

void sendTrig(void)
{
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
}

/****************************************************************************************************/

float32 getDistanceinCM_HCSR04(void)
{
    sendTrig();
    uint32 duration = pulseIn(ECHO_PIN, HIGH);

    uint32 distance = 0.0343 * duration / 2;

    return distance;
}