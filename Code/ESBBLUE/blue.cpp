#include "blue.h"
#include <Arduino.h>


BluetoothSerial Bluetooth_serialESP;

void BLUE_init(void)
{
 Bluetooth_serialESP.begin("Creative-MAT");   
}
