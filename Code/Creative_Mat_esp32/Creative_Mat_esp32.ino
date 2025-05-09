#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#include "bitmaps.h"
#include "application.h"
#include "eerom_map.h"
#include <U8g2lib.h>  // Include U8G2 library for OLED display
//#include <stdint.h>
//#include <stdlib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* I2C SDA */ 21, /* I2C SCL */ 22); // Update with your I2C pins

void setup() {
  Serial.begin(9600);
  
  oled.begin();  // Initialize U8G2 display
  
  oled.clearBuffer();  // Clear the display buffer (for U8G2)
  oled.setFont(u8g2_font_ncenB08_tr);  // Set font (You can change this to match your preference)
  
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);  // Button definitions remain the same
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  
}


void loop() {
  // Buffer to hold the bitmap read from EEPROM
  flag_type flags = { };
  uint8_t NUM_ITEMS = 6;
  menus selected_menu = MainMenu;
  uint8_t item_selected = 0;

  while (1) {
    // Update number of items based on the selected menu
    switch (selected_menu) {
      case MainMenu:
        NUM_ITEMS = 6;
        break;
      case SigGenMenu:
        NUM_ITEMS = 4;
        break;
      case ConfigMenu:
        NUM_ITEMS = 2;
        break;
      default:
        NUM_ITEMS = 1;
    }

    // Button handling
    if (digitalRead(BUTTON_UP_PIN) == LOW) {
      delay(30);
      if (flags.button_up_f == 0) {
        item_selected--;
        if (item_selected == 255) item_selected = NUM_ITEMS - 1;
        flags.button_up_f = 1;
      }
    } else {
      flags.button_up_f = 0;
    }

    if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
      delay(30);
      if (flags.button_down_f == 0) {
        item_selected++;
        if (item_selected >= NUM_ITEMS) item_selected = 0;
        flags.button_down_f = 1;
      }
    } else {
      flags.button_down_f = 0;
    }

    // Display updates
    oled.clearBuffer(); // Clear the buffer before drawing
    switch (selected_menu) {
      case MainMenu:
        oled.drawBitmap(0, 0, epd_bitmap_bg);  // Draw the background bitmap
        drawMenuItem(15, item_selected - 1, MENU_ITEMS_TYPE);  // Draw the previous item
        drawMenuItem(37, item_selected, MENU_ITEMS_TYPE);      // Draw the selected item
        drawMenuItem(59, item_selected + 1, MENU_ITEMS_TYPE);  // Draw the next item
        break;
      
      case VoltmeterMenu:
        oled.setCursor(26, 37);
        oled.print("Voltmeter Reading");
        oled.setCursor(26, 50);
        oled.print("Reading Value");
        break;

      // Add other menu cases like AmmeterMenu, SigGenMenu, etc.
    }
    oled.sendBuffer();  // Send the buffer to the display
    delay(50); // Optional delay for stability
  }
}
