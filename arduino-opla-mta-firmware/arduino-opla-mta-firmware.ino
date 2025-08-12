/*
 * Arduino Opla MTA Firmware
 * Real-time transit and weather information display
 */

#include "config.h"
#include "WiFiManager.h"
#include "LEDManager.h"

// Define API keys
const char* MTA_API_KEY = "your_mta_api_key_here";
const char* WEATHER_API_KEY = "your_openweather_api_key_here";

WiFiManager wifiManager;
LEDManager ledManager;

void setup() {
  Serial.begin(115200);
  Serial.println("Arduino Opla MTA Firmware Starting...");
  
  // Initialize LED manager first for status indication
  ledManager.begin();
  ledManager.setWiFiStatus(WIFI_DISCONNECTED);
  
  // Initialize WiFi
  wifiManager.begin();
  
  Serial.println("Setup complete");
}

void loop() {
  // Update WiFi connection status
  wifiManager.update();
  
  // Update LED status based on WiFi state
  if (wifiManager.isConnected()) {
    ledManager.setWiFiStatus(WIFI_CONNECTED);
  } else if (wifiManager.isConnecting()) {
    ledManager.setWiFiStatus(WIFI_CONNECTING);
  } else {
    ledManager.setWiFiStatus(WIFI_DISCONNECTED);
  }
  
  // Update LED animations
  ledManager.update();
  
  delay(100);
}