#include <Arduino.h>
//#include "Water_Level.h"
#include "oled.h"
//#include "GSM.h"

/* Global Variables *****************************************************************/

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE); // Oled Object
float  x=1.5;
int AllData;
// Struct to store all data initialization

int current_sensor = 0; // Global Variable to quickly choose Depth Sensor
bool measureData = true;          // Flag to measure data

/***************************************************************************/

/* Menu Variables ***********************************************************/
const int n_items = 15;

const unsigned char *menu_icons[n_items] = {
    amm_bitmap,
    menu_icon_battery,
    menu_icon_temp,
    bitmap_icon_3dcube,
    bitmap_icon_dashboard,
    bitmap_icon_battery,
    bitmap_icon_fireworks,
    bitmap_icon_gps_speed,
    bitmap_icon_knob_over_oled,
    bitmap_icon_parksensor,
    bitmap_icon_turbo,
    volt_bitmap,
    ohm_bitmap,
    sig_gen_bitmap,
    config_bitmap};


char items[n_items][25] = {
    {"cat cow"},
    {"child pose"},
    {"straight leg raises"},
    {"cross body stretch"},
    {"piriforms stretch"},
    {"heel raises"},
    {"plank"},
    {"push ups"},
    {"squats"},
    {"leg raises"},
    {"lunges"},
    {"1extra"},
    {"2extra"},
    {"3extra"},
    {"4extra"},};

int previous =0;
int selected = 1;
int next =2;
int current_screen = 0;

// temp measurements
int temp_progress = 0; // temp_progress bar
float temp = 24;       // depth_1iable to increase the temp_progress bar(in cm)
char temp_buffer[10];  // temp_buffer to hold the converted number

// water level measurements
int progress = 0; // progress bar
char buffer[10];  // Buffer to hold the converted number
// water level_2 measurements
int progress2 = 0; // progress bar
char buffer2[10];  // Buffer to hold the converted number

/*************************************************************************/

void setup()
{
  Serial.begin(115200);

  OLED_init();

  /*Oled Config*/
  u8g2.setColorIndex(1); // white color
  u8g2.setBitmapMode(1);
  u8g2.begin();
  Serial.printf("Setup complete\n");
  delay(10);  
  Serial.printf("Setup complete\n");
  delay(10);  
}

void loop()
{


  /************************************************************************************************************/
  Serial.println("Loop running...");

  /* Button Control ************************************************************************/
  if (current_screen == 0)
  {
    //* Up Button
    if (digitalRead(OLED_BUTTON_UP_PIN) == LOW) // Check if Button is pressed
    {
      delay(10);                                  // Debounce
      if (digitalRead(OLED_BUTTON_UP_PIN) == LOW) // Check if Button is still pressed
      {
        selected = selected - 1;
        if (selected < 0)
        {
          selected = n_items - 1;
        }
      }
      Serial.println("Up Button Pressed");
      while (digitalRead(OLED_BUTTON_UP_PIN) == LOW)
      {
        // Wait for Button Release
      }
    }

    //* Down Button
    if (digitalRead(OLED_BUTTON_DOWN_PIN) == LOW) // Check if Button is pressed
    {
      delay(10);                                    // Debounce
      if (digitalRead(OLED_BUTTON_DOWN_PIN) == LOW) // Check if Button is still pressed
      {
        selected = selected + 1;
        if (selected == n_items)
        {
          selected = 0;
        }
      }

      while (digitalRead(OLED_BUTTON_DOWN_PIN) == LOW)
      {
        // Wait for Button Release
      }
    }

    // Compute previous and next safely
    previous = (selected + n_items - 1) % n_items;
    next = (selected + 1) % n_items;

  }

  //* Enter Button
  if ((digitalRead(OLED_BUTTON_SELECT_PIN) == LOW)) // Check if Button is pressed
  {
    delay(10);                                        // Debounce
    if ((digitalRead(OLED_BUTTON_SELECT_PIN) == LOW)) // Check if Button is still pressed
    {
      if (current_screen == 0)
      {
        current_screen = 1;
      }
      else if (current_screen == 1)
      {
        current_screen = 0;
      }
    }

    while (digitalRead(OLED_BUTTON_SELECT_PIN) == LOW)
    {
      // Wait for Button Release
    }
  }
  /* End of Buttons ****************************************************************************************/
  /* OLED Section *******************************************************************************/
  u8g2.setAutoPageClear(1);
  u8g2.firstPage();
  //Serial
      Serial.println("Previous: " + String(previous));
      Serial.println("Selected: " + String(selected));
      Serial.println("Next: " + String(next));
  do
  {
    if (current_screen == 0)
    {
      u8g2.setFont(u8g_font_7x14);

      // Menu Items with icons
      //Previous
      u8g2.drawStr(26, 15 + 0 * 22, items[previous]);
      u8g2.drawXBMP(4, 2 + 0 * 22, 16, 16, menu_icons[previous]);

      //Current
      u8g2.drawStr(26, 15 + 1 * 22, items[selected]);
      u8g2.drawXBMP(4, 2 + 1 * 22, 16, 16, menu_icons[selected]);

      //Next
      u8g2.drawStr(26, 15 + 2 * 22, items[next]);
      u8g2.drawXBMP(4, 2 + 2 * 22, 16, 16, menu_icons[next]);
     
    
      // Selection outline
      u8g2.drawXBMP(0, 22, 128, 20, menu_sel_outline);
      Serial.println(items[selected]);


      // Scrollbar
      u8g2.drawXBMP(120, 0, 8, 64, menu_scrollbar_bckg);
      u8g2.drawBox(125, (64 / n_items) * selected + 6, 3, 8);
    }
    else if (current_screen == 1)
    {
      u8g2.setFont(u8g2_font_helvB08_tr); // consistent font
      u8g2.setColorIndex(1);              // white text

      if (selected == 0)
      {
        dtostrf(x, 6, 2, buffer);
        u8g2.drawXBMP(0, 0, 128, 64, waterlevel_measurement);
        u8g2.drawStr(25, 55, buffer);
        u8g2.setColorIndex(0); u8g2.drawBox(2, 15, 124, 8);
        u8g2.setColorIndex(1); u8g2.drawBox(2, 16, progress, 6);
      }
      else if (selected == 1)
      {
        dtostrf(x, 6, 2, buffer2);
        u8g2.drawXBMP(0, 0, 128, 64, waterlevel_measurement);
        u8g2.drawStr(25, 55, buffer2);
        u8g2.setColorIndex(0); u8g2.drawBox(2, 15, 124, 8);
        u8g2.setColorIndex(1); u8g2.drawBox(2, 16, progress2, 6);
      }
      else if (selected == 2)
      {
        dtostrf(x, 6, 2, temp_buffer);
        u8g2.drawXBMP(0, 0, 128, 64, temp_measurement);
        u8g2.drawStr(25, 55, temp_buffer);
        u8g2.setColorIndex(0); u8g2.drawBox(2, 15, 124, 8);
        u8g2.setColorIndex(1); u8g2.drawBox(2, 16, temp_progress, 6);
      }
    }
    //delay(100);
  } while (u8g2.nextPage());

  u8g2.setAutoPageClear(0);
  /* End of OLED Control ***********************************************************************/


  /* End of Variables Control ***************************************************************************************/

  /* Send Data to web server **********************************************************************/

}