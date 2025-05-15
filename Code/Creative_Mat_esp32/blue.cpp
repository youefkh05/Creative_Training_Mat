#include "blue.h"
#include <Arduino.h>


/* BLUE TEXT START */
const char* training_messages[] = {
  "Let’s strengthen your core! 💪 \nStay consistent and strong!\n",
  "Working on your knees? 🦵 \nTake it slow and steady!\n",
  "Fix that posture! 🧍 \nKeep your chin up and back straight!\n",
  "Time to improve your lumbar support. 🔧 \nStretch carefully.\n",
  "For your lower back — relief is on the way. 🛌 \nHope it helps!\n",
  "Let’s stretch the piriformis muscle. 🧘 \nBe gentle and breathe.\n",
  "Time to plank up! 🏋️ \nChallenge yourself!\n",
  "Push-up practice! 💥 \nHope you have fun!\n",
  "Extra content will be added just for you! 🎁\nExplore and enjoy!\n"
};

const char* driveLinks[] = {
  "https://shorturl.at/QFMTN",
  "https://shorturl.at/CYdpY",
  "https://shorturl.at/5wiR0",
  "https://shorturl.at/BE5zO",
  "https://shorturl.at/lwcMV",
  "https://tinyurl.com/mpdxz4xe",
  "https://tinyurl.com/vu5634yf",
  "https://tinyurl.com/49xr6e6s",
};


const char* program_finish_messages[] = {
  // Problem 0 - Core exercises
  "🔥 Bear plank complete! Core is getting solid!\n",
  "🦅 Bird-Dog done! You're balancing like a pro!\n",
  "🏃‍♂️ Mountain climbers finished! Heart’s pumping!\n",
  "👏 Shoulder taps wrapped! Core on fire!\n",

  // Problem 1 - Knee pain
  "🦵 Heel prop (Left) done! Step by step!\n",
  "🦵 Heel prop (Right) done! Keep moving forward!\n",
  "🦵 Quad sets complete! Strengthening nicely!\n",
  "🎯 Extra1 wrapped up! Well done!\n",

  // Problem 2 - Kyphosis
  "🐦 Bird-Dog again! You're mastering this!\n",
  "🐱 Cat cow done! Spine is feeling great!\n",
  "🦴 Scapular cat finished! Mobility boosted!\n",
  "🧵 Thread the needle done! Great flexibility!\n",

  // Problem 3 - Lumbar flat
  "🐶 Bird-Dog locked in! Lumbar support engaged!\n",
  "🐞 Dead Bug defeated! Core gains incoming!\n",
  "🍑 Glute Bridge done! Strong backside ahead!\n",
  "🌙 Pelvic Tilt complete! Smooth and steady!\n",

  // Problem 4 - Low back pain
  "🦮 Bird-Dog mastered! You’re consistent!\n",
  "🐈‍⬛ Cat cow finished again — spine is happy!\n",
  "🧒 Child Pose complete! Nice and calm.\n",
  "🌟 Extra1 complete! Solid finish!\n",

  // Problem 5 - Piriformis stretch
  "🧘 piriformis stretch done! Keep breathing.\n",
  "🎁 Extra1 checked off! Smooth work!\n",
  "💨 Extra2 complete! You’re on a roll!\n",
  "✨ Extra3 wrapped up! One step better!\n",

  // Problem 6 - Plank
  "🧱 Plank Left done! Rock solid!\n",
  "🧱 Plank Right done! Balance achieved!\n",
  "⚡ Extra2 complete! Core’s on fire!\n",
  "🚀 Extra3 wrapped! Stability improved!\n",

  // Problem 7 - Push ups
  "💥 Push-Up done! Arms of steel!\n",
  "🔁 Extra1 finished! Keep up the grind!\n",
  "🎯 Extra2 checked! You’re killing it!\n",
  "🔥 Extra3 locked in! Push complete!\n",

  // Problem 8 - Extra
  "🎉 Extra1 done! Exploring new stuff!\n",
  "🎊 Extra2 wrapped! Great job!\n",
  "🏁 Extra3 finished! Almost pro level!\n",
  "🏆 Extra4 complete! You’re leveling up!\n"
};



/* BLUE TEXT END */

/* global variables*/
BluetoothSerial Bluetooth_serialESP;

/* global variables end*/

void BLUE_init(void)
{
 Bluetooth_serialESP.begin(BLUE_NAME);   
}

void BLUE_Start(void){
    if (Bluetooth_serialESP.connected()){
        Bluetooth_serialESP.println(BLUE_WELCOME);
        Bluetooth_serialESP.println(MAT_LINK);
    }
}

void BLUE_Problem_intro(int problemidx){
  if (Bluetooth_serialESP.connected()){
        Bluetooth_serialESP.println(training_messages[problemidx]);
        Bluetooth_serialESP.println(driveLinks[problemidx]);
    }
}


void BLUE_Problem_outro(int problemidx){
  if (Bluetooth_serialESP.connected()){
        Bluetooth_serialESP.println(program_finish_messages[problemidx]);
  }
}


void BLUE_MAT(bool program_flag, bool blue_wel_flag){
  static int changeidx = -1;

  if(blueidx==-1){
    return;
  }

  if (Bluetooth_serialESP.connected() && changeidx != blueidx){
    changeidx = blueidx;
    if(currentState == IDLE && program_flag == true){ //program end
      BLUE_Problem_outro(oled_step);
    }
    else if(program_flag == false){
      Bluetooth_serialESP.println(MAT_EXTRA_MES);
    }
    else{

      if(blue_wel_flag==true && changeidx==0){
        BLUE_Problem_intro(oled_step);
      }

      if(hold_flag == false){
        if(oled_step>=RED_OFFSET){
          Bluetooth_serialESP.println(body_part[oled_step-RED_OFFSET]);
          Bluetooth_serialESP.println("PULL!");     
        }
        else{
          Bluetooth_serialESP.println(body_part[oled_step]);
          Bluetooth_serialESP.println("PUSH!");  
        }
          }
      else{
        Bluetooth_serialESP.println("HOLD for");     
        Bluetooth_serialESP.print(HOLD[currentProgram][programStep]);
        Bluetooth_serialESP.println(" seconds ⏲");
      }
      
      Bluetooth_serialESP.print("Step ");
      Bluetooth_serialESP.print(programStep+1);
      Bluetooth_serialESP.print(" of ");
      Bluetooth_serialESP.println(totalSteps);   
    }
  }
}

void BLUE_MAT_WEL(bool program_flag){

  if (Bluetooth_serialESP.connected() && (program_flag==true)){
    BLUE_Start();
  }
}





        
        
        
      



      

