/*
 * Mode Manager for Arduino Opla MTA Firmware
 * Manages different display modes using a modular architecture
 */

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>
#include "config.h"
#include "MTAManager.h"
#include "BaseMode.h"
#include "AmbientDataMode.h"
#include "NYCMTATransitMode.h"

enum DisplayMode {
  MODE_NONE = -1,
  MODE_AMBIENT = 0,
  MODE_TRANSIT = 1,
  MODE_WEATHER = 2  // Future mode
};

class ModeManager {
private:
  MKRIoTCarrier* carrier;
  MTAManager* mtaManager;
  
  // Mode instances
  BaseMode* modes[3];  // Array to hold different modes
  BaseMode* currentMode;
  DisplayMode currentModeType;
  
  // Initialize modes
  void initializeModes();
  void switchToMode(DisplayMode newMode);

public:
  ModeManager(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr);
  ~ModeManager();
  
  void begin();
  void update();
  void handleButtonPress(int buttonIndex);
  DisplayMode getCurrentModeType() { return currentModeType; }
  String getCurrentModeName();
};

#endif