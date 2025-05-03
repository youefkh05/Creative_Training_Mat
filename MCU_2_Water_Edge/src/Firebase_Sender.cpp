#include "Firebase_Sender.h"
#include "esp_wifi.h"
#include "ESPNOW_Reciever/Reciever.h"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

/*
 * Initializes WiFi and Firebase connection.
 * >>> Ensures ESP-NOW compatibility by using WiFi.mode(WIFI_STA)
 */
void Setup_Firebase()
{
    WiFi.mode(WIFI_STA);  // Keep STA mode for ESP-NOW compatibility

    if (strcmp(WIFI_PASSWORD, "open") == 0) {
        WiFi.begin(WIFI_SSID);  // Open network (no password)
    } else {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Secured network
    }

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi...");
        delay(1000);
    }

    Serial.println("__WiFi Connected!__");

    // Configure NTP time synchronization
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

    // Initialize Firebase
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

/*
 * Enables WiFi for Firebase communication.
 * >>> Only activates WiFi when needed to avoid ESP-NOW conflicts.
 */
void Enable_WiFi()
{
    WiFi.mode(WIFI_STA);

    if (strcmp(WIFI_PASSWORD, "open") == 0) {
        WiFi.begin(WIFI_SSID);  // Open network
    } else {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Secured network
    }

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi for Firebase...");
        delay(500);
    }

    Serial.println("WiFi Connected for Firebase!");
}

/*
 * Sends sensor data to Firebase.
 * >>> Pushes structured JSON data including depth and temperature readings.
 */
void Send_Firebase_Data(float depth1, float temp1, float depth2, float temp2)
{
    FirebaseJson json;
    json.set("time", "Text");           // Placeholder for timestamp
    json.set("date", "2025-03-13");     // Static date (replace with dynamic timestamp if needed)
    json.set("depth1", depth1);
    json.set("temp1", temp1);
    json.set("depth2", depth2);
    json.set("temp2", temp2);

    Serial.println("Uploading data to Firebase...");

    if (Firebase.pushJSON(fbdo, "/sensors/data", json)) {
        Serial.println("✅ JSON Data Uploaded Successfully!");
    } else {
        Serial.println("❌ Failed to upload JSON:");
        Serial.println(fbdo.errorReason());
    }
   

}

void Setup_Coexistence() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_ps(WIFI_PS_NONE);  // Disable power saving for stable operation
    Serial.println("✅ WiFi and ESP-NOW Coexistence Mode Enabled!");
}