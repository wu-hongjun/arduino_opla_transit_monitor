/*
 * LED Manager for Arduino Opla MTA Firmware
 * Controls L1 and L2 RGB LEDs for status indication
 */

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
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
  void setWiFiStatusLED(WiFiConnectionStatus status);

public:
  LEDManager();
  void begin();
  void update();
  void setWiFiStatus(WiFiConnectionStatus status);
  void setDataStatus(DataStatus status);
};

#endif