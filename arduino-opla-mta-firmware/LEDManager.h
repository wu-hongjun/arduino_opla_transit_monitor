/*
 * LED Manager for Arduino Opla MTA Firmware
 * Controls L1 and L2 RGB LEDs for status indication
 */

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>
#include "config.h"
#include "WiFiManager.h"
enum DataStatus {
  DATA_ERROR,
  DATA_LOADING,
  DATA_SUCCESS
};

struct RGBColor {
  uint8_t r, g, b;
};

class LEDManager {
private:
  RGBColor currentL1Color;
  RGBColor currentL2Color;
  RGBColor targetL1Color;
  RGBColor targetL2Color;
  
  unsigned long lastUpdate;
  static const unsigned long UPDATE_INTERVAL_MS = 50; // 20 FPS for smooth transitions
  
  // Color definitions
  static const RGBColor RED;
  static const RGBColor YELLOW;
  static const RGBColor GREEN;
  static const RGBColor OFF;
  
  void setL1Color(RGBColor color);
  void setL2Color(RGBColor color);
  void interpolateColor(RGBColor& current, const RGBColor& target, float speed);

public:
  LEDManager();
  void begin();
  void update();
  void setWiFiStatus(WiFiConnectionStatus status);
  void setDataStatus(DataStatus status);
};

#endif