#ifndef VL53L0X_H
#define VL53L0X_H

#include "../std_types.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define TOF_SCL_PIN A5  // SCL pin
#define TOF_SDA_PIN A4  // SDA pin

void VL53L0X_init(void);
// Initializes the VL53L0X sensor

float32 getDistanceinCM_VL53L0X(void);
// Returns the distance measured by the ToF sensor

#endif
