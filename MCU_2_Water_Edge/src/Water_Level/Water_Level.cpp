/*************************************************************************
 * 
 *  Author:     Ameer Louly
 * 
 *  Date:       02/26/2025
 * 
*************************************************************************/

#include "Water_Level.h"
#include "../Sensors/HCSR04.h"
#include "../Sensors/VL53L0X.h"

/***************************************************************************/

void Sensor_init(Sensor_Type sensor)
{
    switch(sensor)
    {
        case HCSR04:
            HCSR04_init();
            break;
        
        case Waterproof:
        //todo: Call Waterproof sensor init function
            break;

        case ToF:
            VL53L0X_init();
            break;
    }
}

/***************************************************************************************/

float32 getDepth_cm(Sensor_Type sensor)
{
    float32 distance = 0;

    switch(sensor)
    {
        case HCSR04:
            distance = getDistanceinCM_HCSR04();
            break;
        
        case Waterproof:
        //todo: Call Waterproof sensor distance function and store in distance
            break;

        case ToF:
            distance = getDistanceinCM_VL53L0X();
            break;
    }

    // return (CONTAINER_DEPTH_CM - distance);
    return distance;
}

/****************************************************************************************/

float32 getDepth_Average_cm(Sensor_Type sensor)
{
    float32 sum = 0;
    for(uint8 i = 0; i < AVERAGE_NUM; i++)
    {
        sum += getDepth_cm(sensor);
    }

    return (sum / AVERAGE_NUM);
}