#include <Arduino.h>
#include "Water_Level/Water_Level.h"
#include "OLED/oled.h"
#include "Firebase_Sender.h"
#include "GSM/GSM.h"

/* Global Variables *****************************************************************/

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE); // Oled Object

Data AllData;
// Struct to store all data initialization

Sensor_Type current_sensor = ToF; // Global Variable to quickly choose Depth Sensor
bool measureData = true;          // Flag to measure data

/***************************************************************************/

/* Menu Variables ***********************************************************/

const unsigned char *menu_icons[3] = {
    menu_icon_waterdrop,
    menu_icon_battery,
    menu_icon_temp};

const int n_items = 3;
char items[n_items][20] = {
    {"Water level 1"},
    {"Water level 2"},
    {"Temperature"}};

int previous;
int selected = 0;
int next;
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
  Serial.begin(9600);
  /*Intializations*/
  if (measureData == true)
  {
    Sensor_init(current_sensor);
  }
  OLED_init();
  Setup_Firebase();
  Setup_Coexistence();
  ESPNOW_Receiver_Init();
  GSM_init();
  GSM_SMS_init();

  /*Oled Config*/
  u8g2.setColorIndex(1); // white color
  u8g2.setBitmapMode(1);
  u8g2.begin();
}

void loop()
{
  // Get Sensor Data
  if (measureData == true)
  {
    AllData.water_level_2 = getDepth_Average_cm(current_sensor); // Measure Water Depth at Water Edge
  }

  /* Wait to Recieve ESP NOW Data *******************************************************************************/

  //! POLLING
  // Serial.println("Recieveing Data...");
  // while(isRecieved == false) {}
  // isRecieved = false;

  // Control Debug LED
  if (isRecieved == true)
  {
    digitalWrite(ESPNOW_DEBUG_LED, LOW);
    isRecieved = false;
  }

  /************************************************************************************************************/

  // Store Sensor Data in structure
  AllData.water_level_1 = receivedData.water_level_1;
  AllData.temp = receivedData.temp;

  // AllData.water_level_2 = getDepth_Average_cm(current_sensor); // Measure Water Depth at Water Edge

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

    // Control Previous and Next
    previous = selected - 1;
    if (previous < 0)
    {
      previous = n_items - 1;
    }
    next = selected + 1;
    if (next >= n_items)
    {
      next = 0;
    }
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
  do
  {
    if (current_screen == 0)
    {
      // Menu Item 1
      u8g2.setFont(u8g_font_7x14);
      u8g2.drawStr(26, 15, items[0]);
      u8g2.drawXBMP(4, 2, 16, 16, menu_icons[0]);

      // Menu Item 2
      u8g2.setFont(u8g_font_7x14);
      u8g2.drawStr(26, 37, items[1]);
      u8g2.drawXBMP(4, 24, 16, 16, menu_icons[1]);

      // Menu Item 3
      u8g2.setFont(u8g_font_7x14);
      u8g2.drawStr(26, 59, items[2]);
      u8g2.drawXBMP(4, 46, 16, 16, menu_icons[2]);

      if (selected == 1)
      {
        u8g2.drawXBMP(0, 22, 128, 20, menu_sel_outline);
      }

      if (selected == 2)
      {
        u8g2.drawXBMP(0, 44, 128, 20, menu_sel_outline);
      }

      if (selected == 0)
      {
        u8g2.drawXBMP(0, 0, 128, 20, menu_sel_outline);
      }

      u8g2.drawXBMP(120, 0, 8, 64, menu_scrollbar_bckg);

      // draw scrollbar handle
      u8g2.drawBox(125, 64 / n_items * selected + 6, 3, 8);
    }

    else if (current_screen == 1 && selected == 2)
    {
      u8g2.setFont(u8g2_font_helvB08_tr);       // font
      dtostrf(AllData.temp, 6, 2, temp_buffer); // Width = 6, Precision = 2 decimal places

      u8g2.setColorIndex(1); // white color
      u8g2.drawXBMP(0, 0, 128, 64, temp_measurement);

      u8g2.drawStr(25, 55, temp_buffer);

      u8g2.setColorIndex(0);
      u8g2.drawBox(2, 15, 124, 8);
      u8g2.setColorIndex(1);
      u8g2.drawBox(2, 16, temp_progress, 6);
    }
    else if (current_screen == 1 and selected == 0)
    {
      u8g2.setFont(u8g2_font_helvB08_tr);           // font
      dtostrf(AllData.water_level_1, 6, 2, buffer); // Width = 6, Precision = 2 decimal places
      u8g2.setColorIndex(1);                        // white color
      u8g2.drawXBMP(0, 0, 128, 64, waterlevel_measurement);

      u8g2.drawStr(25, 55, buffer);

      u8g2.setColorIndex(0);
      u8g2.drawBox(2, 15, 124, 8);
      u8g2.setColorIndex(1);
      u8g2.drawBox(2, 16, progress, 6);
    }
    else if (current_screen == 1 and selected == 1)
    {
      u8g2.setFont(u8g2_font_helvB08_tr);            // font
      dtostrf(AllData.water_level_2, 6, 2, buffer2); // Width = 6, Precision = 2 decimal places
      u8g2.setColorIndex(1);                         // white color
      u8g2.drawXBMP(0, 0, 128, 64, waterlevel_measurement);

      u8g2.drawStr(25, 55, buffer2);

      u8g2.setColorIndex(0);
      u8g2.drawBox(2, 15, 124, 8);
      u8g2.setColorIndex(1);
      u8g2.drawBox(2, 16, progress2, 6);
    }

  } while (u8g2.nextPage());
  u8g2.setAutoPageClear(0);

  /* End of OLED Control********************************************************************************************/

  /* Variables Control ***************************************************************************************/
  // temp += 0.05; //? Change
  // if(temp > 40)
  // {
  //   temp = 24; //? Change
  // }
  if ((temp_progress < 124) && (AllData.temp < 40))
  {
    temp_progress = (AllData.temp * 124) / 40;
  }
  else
  {
    temp_progress = 0;
  }

  // depth_1 +=0.05; //? Change
  // if(depth_1 > 20)
  // {
  //   depth_1 = 5.5; //? Change
  // }
  if (progress < 124 && AllData.water_level_1 < 28)
  {
    progress = (AllData.water_level_1 * 124) / 28;
  }
  else
  {
    // progress = 0;
  }

  if (progress2 < 124 && AllData.water_level_2 < 28)
  {
    progress2 = (AllData.water_level_2 * 124) / 28;
  }
  else
  {
    // progress2 = 0;
  }

  /* End of Variables Control ***************************************************************************************/

  /* Send Data to web server **********************************************************************/

  if (isRecieved == true)
  {
    ESPNOW_Receiver_deInit();                                                             // Deinitialize ESP-NOW before sending data to Firebase
    Send_Firebase_Data(AllData.water_level_1, AllData.temp, AllData.water_level_2, -200); // Send data to Firebase
    GSM_sendData(AllData.water_level_1, AllData.temp, AllData.water_level_2);             // Send data via GSM
    ESPNOW_Receiver_Init();                                                               // Reinitialize ESP-NOW after sending data
    isRecieved = false;                                                                   // Reset the flag after sending data
  }
}