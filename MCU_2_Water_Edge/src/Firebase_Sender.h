/*
    Code by: Amir Sameh
    Date: 7.3.2025

    This file uploads ESP32 sensor data to Firebase Realtime Database
*/
#ifndef FIREBASE_SENDER_H
#define FIREBASE_SENDER_H

#include "common_includes.h"
#include <WiFi.h>
#include <FirebaseESP32.h>

/* WiFi Credentials */
#define WIFI_SSID "H188A"            // AndroidAPCCB0 -- Communication(8310) -- Linksys00431_5GHz -- TP-LINK_B238 -- MyPhone -- Mi_9t -- ZXHNH168N -- H188A
#define WIFI_PASSWORD "r5{K{P6>Pn.2" // A123456a -- open -- open -- 90701702 -- Locked3614 -- 28072004 -- DHBZhF8$92bL -- r5{K{P6>Pn.2

/* Firebase Credentials */
#define API_KEY "AIzaSyC-D17GV3prkmQDssF9ZbXiKGQq2-XLsiE"
#define DATABASE_URL "https://project-0-51c9c-default-rtdb.firebaseio.com"
#define USER_EMAIL "amirsameh770@gmail.com"
#define USER_PASSWORD "123456789"

/* NTP Server Configuration */
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC (2 * 3600) // GMT+2 (Adjust for your timezone)
#define DAYLIGHT_OFFSET_SEC 0     // Change if daylight savings applies

/* Firebase Objects */
extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;

/* Function Prototypes */
void Setup_Firebase();
void Enable_WiFi();
void Send_Firebase_Data(float depth1, float temp1, float depth2, float temp2);
void Setup_Coexistence();
#endif // FIREBASE_SENDER_H
