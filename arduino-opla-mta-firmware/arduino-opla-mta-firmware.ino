/*
 * Arduino Opla MTA Firmware
 * Real-time transit and weather information display
 */

#include "config.h"
#include "WiFiManager.h"
#include "LEDManager.h"
#include "ModeManager.h"
#include "MTAManager.h"
#include <Arduino_MKRIoTCarrier.h>

// Define API keys
const char* MTA_API_KEY = "your_mta_api_key_here";
const char* WEATHER_API_KEY = "your_openweather_api_key_here";

WiFiManager wifiManager;
LEDManager ledManager;
MKRIoTCarrier carrier;
MTAManager mtaManager;
ModeManager modeManager(&carrier, &mtaManager);

void setup() {
  Serial.begin(115200);
  // Wait for serial port to connect with timeout (for standalone operation)
  unsigned long serialTimeout = millis();
  while (!Serial && (millis() - serialTimeout < 3000)) {
    delay(10);
  }
  delay(500); // Brief delay for initialization
  
  Serial.println("=== Arduino Opla MTA Firmware Starting ===");
  Serial.println("Board: MKR WiFi 1010");
  Serial.println("Initializing components...");
  
  // Initialize MKR IoT Carrier
  Serial.println("Starting MKR IoT Carrier...");
  CARRIER_CASE = true; // Set to true for Arduino Opla case
  carrier.begin();
  carrier.display.setRotation(0);
  
  // Initialize LED manager for WiFi status indication
  Serial.println("Starting LED Manager...");
  ledManager.begin();
  ledManager.setWiFiStatus(WIFI_DISCONNECTED);
  
  // Initialize WiFi first (needed for MTA manager)
  Serial.println("Starting WiFi Manager...");
  wifiManager.begin();
  
  // Initialize MTA Manager
  Serial.println("Starting MTA Manager...");
  mtaManager.begin();
  
  // Initialize Mode Manager
  Serial.println("Starting Mode Manager...");
  modeManager.begin();
  
  Serial.println("=== Setup complete ===");
  Serial.println("Touch button 0 to enter Temperature/Humidity mode");
}

void loop() {
  static unsigned long lastDebug = 0;
  static unsigned long lastModeUpdate = 0;
  
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
  
  // Update button states
  carrier.Buttons.update();
  
  // Check for button presses
  if (carrier.Buttons.onTouchDown(TOUCH0)) {
    modeManager.handleButtonPress(0);
  }
  if (carrier.Buttons.onTouchDown(TOUCH1)) {
    modeManager.handleButtonPress(1);
  }
  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    modeManager.handleButtonPress(2);
  }
  if (carrier.Buttons.onTouchDown(TOUCH3)) {
    modeManager.handleButtonPress(3);
  }
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    modeManager.handleButtonPress(4);
  }
  
  // Update mode display every 5 seconds (less frequent updates)
  if (millis() - lastModeUpdate > 5000) {
    modeManager.update();
    lastModeUpdate = millis();
  }
  
  // Debug output every 10 seconds
  if (millis() - lastDebug > 10000) {
    Serial.print("Status: WiFi=");
    if (wifiManager.isConnected()) {
      Serial.print("CONNECTED");
    } else if (wifiManager.isConnecting()) {
      Serial.print("CONNECTING");
    } else {
      Serial.print("DISCONNECTED");
    }
    Serial.print(", Mode=");
    Serial.print(modeManager.getCurrentMode());
    Serial.print(", State=");
    Serial.print(modeManager.getCurrentState());
    Serial.print(", Uptime=");
    Serial.print(millis() / 1000);
    Serial.println("s");
    lastDebug = millis();
  }
  
  delay(50);
}