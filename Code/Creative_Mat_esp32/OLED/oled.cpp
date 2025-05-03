#include "oled.h"
#include <Arduino.h>

void OLED_init(void)
{
    pinMode(OLED_BUTTON_UP_PIN , INPUT_PULLUP);
    pinMode(OLED_BUTTON_SELECT_PIN, INPUT_PULLUP);
    pinMode(OLED_BUTTON_DOWN_PIN, INPUT_PULLUP);
}