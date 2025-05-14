#include "blue.h"
#include <Arduino.h>


/* BLUE TEXT START */
const char* training_messages[] = {
  "Let’s strengthen your core! 💪 \nStay consistent and strong!",
  "Working on your knees? 🦵 \nTake it slow and steady!",
  "Fix that posture! 🧍 \nKeep your chin up and back straight!",
  "Time to improve your lumbar support. 🔧 \nStretch carefully.",
  "For your lower back — relief is on the way. 🛌 \nHope it helps!",
  "Let’s stretch the piriformis muscle. 🧘 \nBe gentle and breathe.",
  "Time to plank up! 🏋️ \nChallenge yourself!",
  "Push-up practice! 💥 \nHope you have fun!",
  "Extra content will be added just for you! 🎁\nExplore and enjoy!"
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
  "🔥 Bear plank complete! Core is getting solid!",
  "🦅 Bird-Dog done! You're balancing like a pro!",
  "🏃‍♂️ Mountain climbers finished! Heart’s pumping!",
  "👏 Shoulder taps wrapped! Core on fire!",

  // Problem 1 - Knee pain
  "🦵 Heel prop (Left) done! Step by step!",
  "🦵 Heel prop (Right) done! Keep moving forward!",
  "🦵 Quad sets complete! Strengthening nicely!",
  "🎯 Extra1 wrapped up! Well done!",

  // Problem 2 - Kyphosis
  "🐦 Bird-Dog again! You're mastering this!",
  "🐱 Cat cow done! Spine is feeling great!",
  "🦴 Scapular cat finished! Mobility boosted!",
  "🧵 Thread the needle done! Great flexibility!",

  // Problem 3 - Lumbar flat
  "🐶 Bird-Dog locked in! Lumbar support engaged!",
  "🐞 Dead Bug defeated! Core gains incoming!",
  "🍑 Glute Bridge done! Strong backside ahead!",
  "🌙 Pelvic Tilt complete! Smooth and steady!",

  // Problem 4 - Low back pain
  "🦮 Bird-Dog mastered! You’re consistent!",
  "🐈‍⬛ Cat cow finished again — spine is happy!",
  "🧒 Child Pose complete! Nice and calm.",
  "🌟 Extra1 complete! Solid finish!",

  // Problem 5 - Piriformis stretch
  "🧘 piriformis stretch done! Keep breathing.",
  "🎁 Extra1 checked off! Smooth work!",
  "💨 Extra2 complete! You’re on a roll!",
  "✨ Extra3 wrapped up! One step better!",

  // Problem 6 - Plank
  "🧱 Plank Left done! Rock solid!",
  "🧱 Plank Right done! Balance achieved!",
  "⚡ Extra2 complete! Core’s on fire!",
  "🚀 Extra3 wrapped! Stability improved!",

  // Problem 7 - Push ups
  "💥 Push-Up done! Arms of steel!",
  "🔁 Extra1 finished! Keep up the grind!",
  "🎯 Extra2 checked! You’re killing it!",
  "🔥 Extra3 locked in! Push complete!",

  // Problem 8 - Extra
  "🎉 Extra1 done! Exploring new stuff!",
  "🎊 Extra2 wrapped! Great job!",
  "🏁 Extra3 finished! Almost pro level!",
  "🏆 Extra4 complete! You’re leveling up!"
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
    if (Bluetooth_serialESP.available()){
        Bluetooth_serialESP.println(BLUE_WELCOME);
        Bluetooth_serialESP.println(MAT_LINK);
    }
}

void BLUE_Problem_intro(int problemidx){
  if (Bluetooth_serialESP.available()){
        Bluetooth_serialESP.println(training_messages[problemidx]);
        Bluetooth_serialESP.println(driveLinks[problemidx]);
    }
}

void BLUE_Problem_outro(int problemidx){
  if (Bluetooth_serialESP.available()){
        Bluetooth_serialESP.println(program_finish_messages[problemidx]);
  }
}

void BLUE_MAT(bool program_flag){
  static int changeidx = -1;

  if(blueidx==-1){
    return;
  }

  if (Bluetooth_serialESP.available() && changeidx != blueidx){
    changeidx = blueidx;
    if(currentState == IDLE && program_flag == true){ //program end
      BLUE_Problem_outro(oled_step);
    }
    else if(program_flag == false){
      Bluetooth_serialESP.println(MAT_EXTRA_MES);
    }
    else{
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





        
        
        
      



      

