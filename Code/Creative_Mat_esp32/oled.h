/**
 *      Author: Tesla, Shahd, Kareem
 *      Date: 5/3/2025
 */

#ifndef OLED_H
#define OLED_H

#include "bitmaps.h"
#include "U8g2lib.h"

#define OLED_SDA_PIN                21
#define OLED_SCL_PIN                22

#define OLED_BUTTON_UP_PIN          34
#define OLED_BUTTON_SELECT_PIN      35
#define OLED_BUTTON_DOWN_PIN        36

void OLED_init(void);

#endif