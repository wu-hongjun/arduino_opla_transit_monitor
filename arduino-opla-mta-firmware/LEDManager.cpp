#include "LEDManager.h"
#include "WiFiManager.h"

// No color definitions needed for built-in LED

LEDManager::LEDManager() {
  // Constructor - nothing needed for simple built-in LED control
}

void LEDManager::begin() {
  // Initialize built-in RGB LED pins using WiFiDrv
  WiFiDrv::pinMode(25, OUTPUT); // Green LED
  WiFiDrv::pinMode(26, OUTPUT); // Red LED  
  WiFiDrv::pinMode(27, OUTPUT); // Blue LED
  
  Serial.println("LED Manager initialized with MKR WiFi 1010 built-in RGB LED");
  
  // Set initial state (WiFi disconnected = red)
  setWiFiStatusLED(WIFI_DISCONNECTED);
}

void LEDManager::update() {
  // No longer needed for simple built-in LED control
}

void LEDManager::setWiFiStatus(WiFiConnectionStatus status) {
  setWiFiStatusLED(status);
}

void LEDManager::setDataStatus(DataStatus status) {
  // For now, we'll just log data status since we're using the built-in LED for WiFi only
  Serial.print("Data Status: ");
  switch (status) {
    case DATA_ERROR:
      Serial.println("ERROR");
      break;
    case DATA_LOADING:
      Serial.println("LOADING");
      break;
    case DATA_SUCCESS:
      Serial.println("SUCCESS");
      break;
  }
}

void LEDManager::setWiFiStatusLED(WiFiConnectionStatus status) {
  static WiFiConnectionStatus lastStatus = WIFI_DISCONNECTED;
  
  // Only update LED and print if status changed
  if (status != lastStatus) {
    switch (status) {
      case WIFI_DISCONNECTED:
        // Red LED on (pin 26)
        WiFiDrv::digitalWrite(25, LOW);  // Green off
        WiFiDrv::digitalWrite(26, HIGH); // Red on
        WiFiDrv::digitalWrite(27, LOW);  // Blue off
        Serial.println("WiFi Status LED: RED (Disconnected)");
        break;
        
      case WIFI_CONNECTING:
        // Yellow LED (Red + Green on)
        WiFiDrv::digitalWrite(25, HIGH); // Green on
        WiFiDrv::digitalWrite(26, HIGH); // Red on  
        WiFiDrv::digitalWrite(27, LOW);  // Blue off
        Serial.println("WiFi Status LED: YELLOW (Connecting)");
        break;
        
      case WIFI_CONNECTED:
        // Green LED on (pin 25)
        WiFiDrv::digitalWrite(25, HIGH); // Green on
        WiFiDrv::digitalWrite(26, LOW);  // Red off
        WiFiDrv::digitalWrite(27, LOW);  // Blue off
        Serial.println("WiFi Status LED: GREEN (Connected)");
        break;
    }
    
    lastStatus = status;
  }
}