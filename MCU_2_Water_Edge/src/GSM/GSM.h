#ifndef GSM_H
#define GSM_H

#include "../std_types.h"
#include <Arduino.h>

#define PHONE_NUMBER "+201144465559" // +201060930725 (Ziad Emad) -- +201287077844 (Ameer) -- +201144465559 (Kareem)

void GSM_init(void);
// Initializes the GSM module

void GSM_SMS_init(void);
// Initializes the SMS functionality of the GSM module

void GSM_sendSMS(char *message);
// Sends an SMS message to the specified phone number

void GSM_sendData(float depth1, float temp1, float depth2);
// Sends sensor data via SMS

#endif // GSM_H