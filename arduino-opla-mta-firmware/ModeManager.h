/*
 * Mode Manager for Arduino Opla MTA Firmware
 * Handles different display modes and their states using Arduino_OplaUI
 */

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_OplaUI.h>
#include "config.h"
#include "MTAManager.h"
#include "RadialDisplay.h"

enum DisplayMode {
  MODE_NONE = -1,
  MODE_TEMP_HUMIDITY = 0,
  MODE_TRANSIT = 1,
  MODE_WEATHER = 2
};

enum TempHumidityState {
  TEMP_CELSIUS = 1,
  TEMP_FAHRENHEIT = 2
};

enum TransitState {
  TRANSIT_UPTOWN = 1,
  TRANSIT_DOWNTOWN = 2
};

enum DisplayTheme {
  THEME_LIGHT = 0,
  THEME_DARK = 1
};

class ModeManager {
private:
  MKRIoTCarrier* carrier;
  MTAManager* mtaManager;
  RadialDisplay* radialDisplay;
  DisplayMode currentMode;
  int currentState;
  DisplayTheme currentTheme;
  
  // Opla UI Gauges
  OplaGauge1 tempGauge;
  OplaGauge2 humidityGauge;
  
  // Theme management
  void updateTheme();
  DisplayTheme getThemeFromLightSensor();
  void applyThemeToGauges();
  
  // Weather mode (combined temp, humidity, pressure, light)
  void displayWeather();
  void updateWeatherState();
  void initializeGauges();
  void drawRadialWeatherDisplay(float temperature, String tempUnit, float humidity, int lightLevel);
  
  // Transit mode
  void displayTransit();
  void updateTransitState();

public:
  ModeManager(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr);
  void begin();
  void update();
  void handleButtonPress(int buttonIndex);
  DisplayMode getCurrentMode();
  int getCurrentState();
};

#endif