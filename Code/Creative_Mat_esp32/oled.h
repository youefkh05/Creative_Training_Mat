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

/* screen constants*/

#define MAX_TEXT_NUM                14 // max letters on menu list
#define MAX_BAR_NUM                 9  // number of points on the bar
#define FIRST_RAW                   40 // FIRST RAW y position
#define SECOND_RAW                  55 // SECOND RAW y position
#define BAR                         15 // progress bar y position
#define SIGN                         80 // Sign x position

void OLED_init(void);

#endif