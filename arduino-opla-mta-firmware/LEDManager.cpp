#include "LEDManager.h"
#include "WiFiManager.h"

// Color definitions
const RGBColor LEDManager::RED = {255, 0, 0};
const RGBColor LEDManager::YELLOW = {255, 255, 0};
const RGBColor LEDManager::GREEN = {0, 255, 0};
const RGBColor LEDManager::OFF = {0, 0, 0};

LEDManager::LEDManager() {
  currentL1Color = OFF;
  currentL2Color = OFF;
  targetL1Color = OFF;
  targetL2Color = OFF;
  lastUpdate = 0;
}

void LEDManager::begin() {
  // Initialize LED pins - Note: Arduino Opla uses different LED control
  // This is a placeholder implementation for basic testing
  Serial.println("LED Manager initialized");
  
  // Set initial state
  setL1Color(OFF);
  setL2Color(OFF);
}

void LEDManager::update() {
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_INTERVAL_MS) {
    // Smooth color transitions
    interpolateColor(currentL1Color, targetL1Color, 0.1f);
    interpolateColor(currentL2Color, targetL2Color, 0.1f);
    
    lastUpdate = now;
  }
}

void LEDManager::setWiFiStatus(WiFiConnectionStatus status) {
  switch (status) {
    case WIFI_DISCONNECTED:
      targetL1Color = RED;
      break;
    case WIFI_CONNECTING:
      targetL1Color = YELLOW;
      break;
    case WIFI_CONNECTED:
      targetL1Color = GREEN;
      break;
  }
}

void LEDManager::setDataStatus(DataStatus status) {
  switch (status) {
    case DATA_ERROR:
      targetL2Color = RED;
      break;
    case DATA_LOADING:
      targetL2Color = YELLOW;
      break;
    case DATA_SUCCESS:
      targetL2Color = GREEN;
      break;
  }
}

void LEDManager::setL1Color(RGBColor color) {
  // Placeholder implementation - replace with actual Arduino Opla LED control
  Serial.print("L1 LED: R=");
  Serial.print(color.r);
  Serial.print(" G=");
  Serial.print(color.g);
  Serial.print(" B=");
  Serial.println(color.b);
}

void LEDManager::setL2Color(RGBColor color) {
  // Placeholder implementation - replace with actual Arduino Opla LED control
  Serial.print("L2 LED: R=");
  Serial.print(color.r);
  Serial.print(" G=");
  Serial.print(color.g);
  Serial.print(" B=");
  Serial.println(color.b);
}

void LEDManager::interpolateColor(RGBColor& current, const RGBColor& target, float speed) {
  current.r = current.r + (target.r - current.r) * speed;
  current.g = current.g + (target.g - current.g) * speed;
  current.b = current.b + (target.b - current.b) * speed;
  
  // Update actual LED if color changed significantly
  static RGBColor lastL1 = {255, 255, 255}; // Force initial update
  static RGBColor lastL2 = {255, 255, 255};
  
  if (abs(current.r - lastL1.r) > 5 || abs(current.g - lastL1.g) > 5 || abs(current.b - lastL1.b) > 5) {
    if (&current == &currentL1Color) {
      setL1Color(current);
      lastL1 = current;
    }
  }
  
  if (abs(current.r - lastL2.r) > 5 || abs(current.g - lastL2.g) > 5 || abs(current.b - lastL2.b) > 5) {
    if (&current == &currentL2Color) {
      setL2Color(current);
      lastL2 = current;
    }
  }
}