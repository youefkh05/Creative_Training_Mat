/*
 * espnow_receiver.c
 *
 *  Created on: Feb 18, 2025
 *      Author: Zaid Mostafa (Embedded legends member)
 *
 *  This file implements the ESP-NOW receiver functions.
 */

#include "Reciever.h"
#include "../Water_Level/Water_Level.h"

/* Variable to hold received sensor data */
SensorData receivedData;
volatile bool isRecieved = false;

/*
 * Callback function triggered when data is received.
 * - Copies received data into `receivedData` struct.
 * - Prints the received values to the Serial Monitor.
 * - Uploads the data to the Blynk dashboard.
 */
volatile void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    //digitalWrite(ESPNOW_DEBUG_LED, HIGH); // Turn on LED to indicate data reception
    memcpy(&receivedData, incomingData, sizeof(receivedData)); // Copy received data
    // Serial.printf("Received: Water Level = %.2f, Temperature = %.2f\n", receivedData.water_level_1, receivedData.temp);
    isRecieved = true;

      // Print Water Level
    Serial.printf("\nWater Level 1: %.2f\n", AllData.water_level_1);
    Serial.printf("Water Level 2: %.2f\n", AllData.water_level_2);
    Serial.printf("Temperature: %.2f\n\n", AllData.temp);
    // Send_Firebase_Data(AllData.water_level_1, AllData.temp, AllData.water_level_2, -200);
    // Send Data to Firebase
}

/*
 * Initializes ESP-NOW on the receiver ESP32.
 * - Initializes ESP-NOW and registers a receive callback.
 */
void ESPNOW_Receiver_Init()
{
    WiFi.mode(WIFI_AP_STA); // Set ESP32 to station mode

    //digitalWrite(ESPNOW_DEBUG_LED, HIGH); // Turn on LED to indicate ESP-NOW initialization
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed\n");
        return;
    }

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv)); // Register receive callback
}

void ESPNOW_Receiver_deInit()
{
    esp_now_deinit(); // Deinitialize ESP-NOW
}

// void Enable_WiFi()
// {
//     WiFi.mode(WIFI_STA);              // >>> HIGHLIGHT: Ensure STA mode remains active
//     WiFi.begin("WIFI_SSID", "WIFI_PASSWORD"); // >>> HIGHLIGHT: Replace with  actual credentials

//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.println("Connecting to WiFi for Firebase...");
//         delay(1000);
//     }
//     Serial.println("WiFi Connected for Firebase!");  // >>> HIGHLIGHT: WiFi connection established
// }

/*
 * Switches back to ESP-NOW mode after Firebase upload.
 * >>> HIGHLIGHT: Disconnects from the WiFi network to resume ESP-NOW operation.
 */
void Enable_ESPNow()
{
    WiFi.disconnect(true);  // >>> HIGHLIGHT: Disconnect from WiFi to prevent interference with ESP-NOW
    WiFi.mode(WIFI_STA);      // >>> HIGHLIGHT: Maintain STA mode for ESP-NOW
    Serial.println("Switched back to ESP-NOW mode (WiFi disconnected).");  // >>> HIGHLIGHT: Confirmation message
}





