/*************************************************************************
 * 
 *  Author:     Ameer Louly
 * 
 *  Date:       02/26/2025
 * 
*************************************************************************/
#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

#include "../common_includes.h"
#include "../ESPNOW_Reciever/Reciever.h"


#define CONTAINER_DEPTH_CM      15
#define AVERAGE_NUM             5

typedef enum
{
    HCSR04,
    Waterproof,
    ToF
} Sensor_Type;

struct Data : public SensorData
{
  float32 water_level_2;
};

extern Data AllData; 
//todo: Add Structure data type to hold measured data

void Sensor_init(Sensor_Type sensor);

float32 getDepth_cm(Sensor_Type sensor);
// Returns measured depth according to sensor type

float32 getDepth_Average_cm(Sensor_Type sensor);
// Returns the average of the last 5 readings

//todo: Add temprature sensor functions

#endif

