#include "VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void VL53L0X_init(void)
{
    Wire.begin(); // Initialize I2C
    if (!lox.begin())
    {
        Serial.println("Failed to initialize VL53L0X!");
        while (1);
    }
}

/*****************************************************************************************************/

float32 getDistanceinCM_VL53L0X(void)
{
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) // If valid measurement
    {
        return (float32)measure.RangeMilliMeter / 10.0; // Convert mm to cm
    }
    else
    {
        return -1; // Return -1 if out of range
    }
}
