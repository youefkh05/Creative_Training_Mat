#include "oled.h"
#include <Arduino.h>

void OLED_init(void)
{
    pinMode(OLED_BUTTON_UP_PIN , INPUT);
    pinMode(OLED_BUTTON_SELECT_PIN, INPUT);
    pinMode(OLED_BUTTON_DOWN_PIN, INPUT);
    #ifdef DEBUG
        Serial.println("Oled Init");
        vTaskDelay(500 / portTICK_PERIOD_MS);  // Non-blocking delay
    #endif
}