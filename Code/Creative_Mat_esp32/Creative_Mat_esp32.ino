#define ESP32 

#ifdef ESP32
  #define PROGMEM
#endif

#include <Arduino.h>
#include "mat.h"
#include "oled.h"


/* Global Variables *****************************************************************/

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE); // Oled Object
bool startmat=false;
bool program_flag = true;

char buffer[10];
/***************************************************************************/

/* Menu Variables ***********************************************************/
const int n_items = 10;
const int n_items2 = TrainingPerProblem;

const unsigned char* const menu_icons[n_items] PROGMEM = {
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
};


const char *items[n_items] = {
    training_problem[0],
    training_problem[1],
    training_problem[2],
    training_problem[3],
    training_problem[4],
    training_problem[5],
    training_problem[6],
    training_problem[7],
    training_problem[8],
    {"connect device"},
};

int previous =0;
int selected = 1;
int next =2;
int previous2 =0;
int selected2 = 1;
int next2 =2;
int current_screen = 0;


/*************************************************************************/


/* end of helper functions*/

void setup()
{
  //Serial.begin(115200);

  OLED_init();

  /*Oled Config*/
  u8g2.setColorIndex(1); // white color
  u8g2.setBitmapMode(1);
  u8g2.begin();
  mat_init();

  #ifdef DEBUG
    Serial.println("Start");
    delay(500);
    Serial.printf("Setup complete\n");
  #endif
   
}

void loop()
{


  /************************************************************************************************************/
  //Serial.println("Loop running...");

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
      //Serial.println("Up Button Pressed");
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

  if (current_screen == 1)
  {
    //* Up Button
    if (digitalRead(OLED_BUTTON_UP_PIN) == LOW) // Check if Button is pressed
    {
      delay(10);                                  // Debounce
      if (digitalRead(OLED_BUTTON_UP_PIN) == LOW) // Check if Button is still pressed
      {
        selected2 = selected2 - 1;
        if (selected2 < 0)
        {
          selected2 = n_items2 - 1;
        }
      }
      //Serial.println("Up Button Pressed");
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
        selected2 = selected2 + 1;
        if (selected2 == n_items2)
        {
          selected2 = 0;
        }
      }

      while (digitalRead(OLED_BUTTON_DOWN_PIN) == LOW)
      {
        // Wait for Button Release
      }
    }

    // Compute previous and next safely
    previous2 = (selected2 + n_items2 - 1) % n_items2;
    next2 = (selected2 + 1) % n_items2;

  }

  //* Enter Button
  if ((digitalRead(OLED_BUTTON_SELECT_PIN) == LOW)) // Check if Button is pressed
  {
    delay(10);                                        // Debounce
    if ((digitalRead(OLED_BUTTON_SELECT_PIN) == LOW)) // Check if Button is still pressed
    {
      setProgram((selected*n_items2) + selected2, false);
      if (current_screen == 0)
      {
        startmat = false;
        program_flag = true;
        current_screen = 1;
      }
      else if (current_screen == 1)
      {
        startmat = true;
        program_flag = true;
        if(PROGRAMS[currentProgram][0] == -1){
          program_flag = false;
        }
        current_screen = 2;
      }
      else if (current_screen == 2)
      {
        startmat = false;
        program_flag = true;
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
  ////Serial
      //Serial.println("Previous: " + String(previous));
      //Serial.println("Selected: " + String(selected));
      //Serial.println("Next: " + String(next));
  do
  {
    if (current_screen == 0)
    {
      u8g2.setFont(u8g_font_7x14);

      // Menu Items with icons
      //Previous
      u8g2.drawStr(26, 15 + 0 * 22, items[previous]);


      u8g2.drawXBMP(4, 2 + 0 * 22, icon_width_big, icon_height_big , menu_icons[previous]);

      //Current
      u8g2.drawStr(26, 15 + 1 * 22, items[selected]);
      u8g2.drawXBMP(4, 2 + 1 * 22, icon_width_big  , icon_height_big, menu_icons[selected]);

      //Next
      u8g2.drawStr(26, 15 + 2 * 22, items[next]);
      u8g2.drawXBMP(4, 2 + 2 * 22, icon_width_big  , icon_height_big, menu_icons[next]);
     
    
      // Selection outline
      u8g2.drawXBMP(0, 22, 128, 20, menu_sel_outline);
      //Serial.println(items[selected]);


      // Scrollbar
      u8g2.drawXBMP(120, 0, 8, 64, menu_scrollbar_bckg);
      u8g2.drawBox(125, (64 / n_items) * selected + 6, 3, 8);
    }
    else if (current_screen == 1)
    {
      u8g2.setFont(u8g_font_7x14);

      // Menu2 Items with icons
      //Previous
      u8g2.drawStr(26, 15 + 0 * 22, problem_program[(selected*n_items2) + previous2]);


      u8g2.drawXBMP(4, 2 + 0 * 22, icon_width_big, icon_height_big , menu_icons[previous2]);

      //Current
      u8g2.drawStr(26, 15 + 1 * 22, problem_program[(selected*n_items2) + selected2]);
      u8g2.drawXBMP(4, 2 + 1 * 22, icon_width_big  , icon_height_big, menu_icons[selected2]);

      //Next
      u8g2.drawStr(26, 15 + 2 * 22, problem_program[(selected*n_items2) + next2]);
      u8g2.drawXBMP(4, 2 + 2 * 22, icon_width_big  , icon_height_big, menu_icons[next2]);
     
    
      // Selection outline
      u8g2.drawXBMP(0, 22, 128, 20, menu_sel_outline);
      //Serial.println(items[selected]);


      // Scrollbar
      u8g2.drawXBMP(120, 0, 8, 64, menu_scrollbar_bckg);
      u8g2.drawBox(125, (64 / n_items2) * selected2 + 6, 3, 8);
    }
    else if (current_screen == 2)
    {
      u8g2.setFont(u8g2_font_helvB08_tr); // consistent font
      u8g2.setColorIndex(1);              // white text

      if (selected == (n_items-1))
      {
        u8g2.drawXBMP(0, 0, image_width_big, image_height_big, training_screen);
        u8g2.setColorIndex(0); u8g2.drawBox(2, 15, 124, 8);
      }
      else 
      {

        if(startmat == true){
          setProgram((selected*n_items2) + selected2, true);
          startmat = false;
        }

        if(currentState == IDLE && program_flag == true){ //program end
          u8g2.drawStr(10, FIRST_RAW, "Training");
          u8g2.drawStr(1, SECOND_RAW, "finished!");
          u8g2.drawXBMP(SIGN, BAR+12, sign_width_big  , sign_height_big, correct_sign);
          u8g2.setColorIndex(0); u8g2.drawBox(2, BAR, 124, 8);
          u8g2.setColorIndex(1); u8g2.drawBox(2, BAR +1, 124, 6);
          setProgram((selected*n_items2) + selected2, false);
        }
        else if(program_flag == false){
          u8g2.drawStr(10, FIRST_RAW, "NO Training");
          u8g2.drawStr(1, SECOND_RAW, "it'll be added!");
          u8g2.drawXBMP(SIGN, BAR+12, sign_width_big  , sign_height_big, happy_sign);
          u8g2.setColorIndex(0); u8g2.drawBox(2, BAR, 124, 8);
          u8g2.setColorIndex(1); u8g2.drawBox(2, BAR +1, 0, 6);
          //setProgram((selected*n_items2) + selected2, false);
        }
        else{
          if(hold_flag == false){
            if(oled_step>=RED_OFFSET){
              u8g2.drawStr(10, SECOND_RAW, "PULL!");
              u8g2.drawStr(1, FIRST_RAW, body_part[oled_step-RED_OFFSET]);
              u8g2.drawXBMP(SIGN, BAR+12, sign_width_big  , sign_height_big, up_sign);
            }
            else{
              u8g2.drawStr(10, SECOND_RAW, "PUSH!");
              u8g2.drawStr(1, FIRST_RAW, body_part[oled_step]);
              u8g2.drawXBMP(SIGN, BAR+12, sign_width_big  , sign_height_big, down_sign);
            }
          }
          else{
            u8g2.drawStr(1, FIRST_RAW, "HOLD for");
            sprintf(buffer, "%d", HOLD[currentProgram][programStep]);
            strcat(buffer, " sec");
            u8g2.drawStr(10, SECOND_RAW, buffer);
            u8g2.drawXBMP(SIGN, BAR+12, sign_width_big  , sign_height_big, clock_sign);
          }
        u8g2.setColorIndex(0); u8g2.drawBox(2, BAR, 124, 8);
        u8g2.setColorIndex(1); u8g2.drawBox(2, BAR +1, 124 * (programStep/(float)totalSteps), 6);
        }
      

        u8g2.drawXBMP(0, 0, image_width_big, image_height_big, training_screen);
        u8g2.setFont(u8g_font_7x14);
        for(int i = 0; i < MAX_BAR_NUM; i++){
          sprintf(buffer, "%d", map(i,0,MAX_BAR_NUM-1,0,totalSteps));  
          u8g2.drawStr(1+15*i, 10, buffer);
        }

      }

    }
    //delay(100);
  } while (u8g2.nextPage());

  u8g2.setAutoPageClear(0);
  /* End of OLED Control ***********************************************************************/

  mat_loop();
  /* End of Variables Control ***************************************************************************************/

  /* Send Data to web server **********************************************************************/

}

