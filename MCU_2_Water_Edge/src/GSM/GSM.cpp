#include "GSM.h"
#include <stdlib.h>
#include <string.h>

HardwareSerial SIM800(2);

void GSM_init(void)
{
    SIM800.begin(9600, SERIAL_8N1, 16, 17); // RX, TX pins
    delay(500);
    Serial.println("GSM Module Initialized");
}

void GSM_SMS_init(void)
{
    SIM800.println("AT+CMGF=1"); // Set SMS mode to text
    delay(1000);
    Serial.println("SMS Mode Set to Text");
}

void GSM_sendData(float depth1, float temp1, float depth2)
{
    char message[100];
    snprintf(message, sizeof(message), "Sender:depth1=%.2fcm,temp=%.2f C, depth2 = %.2fcm", depth1, temp1, depth2);
    GSM_sendSMS(message);
}

void GSM_sendSMS(char *message)
{
    SIM800.println("AT+CMGS=\"" PHONE_NUMBER "\""); // Set recipient number
    delay(50);

    if (SIM800.available())
    {
        Serial.write(SIM800.read()); // Print response for debugging
    }

    SIM800.println(message); // Send message
    delay(50);

    SIM800.write(26); // Send Ctrl+Z to indicate end of message
    delay(50);

    if (SIM800.available())
    {
        Serial.write(SIM800.read()); // Print response for debugging
    }

    Serial.println("SMS Sent");
}