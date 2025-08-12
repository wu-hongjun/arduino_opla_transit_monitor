#include "ModeManager.h"

ModeManager::ModeManager(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr) {
  carrier = carrierPtr;
  mtaManager = mtaPtr;
  currentMode = nullptr;
  currentModeType = MODE_NONE;
  
  // Initialize mode pointers to nullptr
  for (int i = 0; i < 3; i++) {
    modes[i] = nullptr;
  }
}

ModeManager::~ModeManager() {
  // Clean up mode instances
  for (int i = 0; i < 3; i++) {
    if (modes[i] != nullptr) {
      delete modes[i];
    }
  }
}

void ModeManager::begin() {
  Serial.println("Mode Manager initialized");
  initializeModes();
  
  // Start with ambient mode
  switchToMode(MODE_AMBIENT);
}

void ModeManager::initializeModes() {
  // Create mode instances
  modes[MODE_AMBIENT] = new AmbientDataMode(carrier);
  modes[MODE_TRANSIT] = new NYCMTATransitMode(carrier, mtaManager);
  modes[MODE_WEATHER] = nullptr;  // Future implementation
  
  Serial.println("Modes initialized");
}

void ModeManager::update() {
  if (currentMode != nullptr) {
    currentMode->update();
  }
}

void ModeManager::handleButtonPress(int buttonIndex) {
  Serial.print("Button pressed: ");
  Serial.println(buttonIndex);
  
  switch (buttonIndex) {
    case 0: // TOUCH0 - Ambient Data mode
      if (currentModeType == MODE_AMBIENT) {
        // Pass button press to current mode for internal state changes
        currentMode->handleButtonPress(buttonIndex);
      } else {
        switchToMode(MODE_AMBIENT);
      }
      break;
      
    case 1: // TOUCH1 - Transit mode
      if (currentModeType == MODE_TRANSIT) {
        // Pass button press to current mode for internal state changes
        currentMode->handleButtonPress(buttonIndex);
      } else {
        switchToMode(MODE_TRANSIT);
      }
      break;
      
    case 2: // TOUCH2 - Future: Weather mode  
      Serial.println("Weather mode not implemented yet");
      break;
      
    case 3: // TOUCH3 - Future: Additional mode
      Serial.println("Additional mode not implemented yet");
      break;
      
    case 4: // TOUCH4 - Future: Settings/Exit
      Serial.println("Settings mode not implemented yet");
      break;
  }
}

void ModeManager::switchToMode(DisplayMode newMode) {
  // Validate mode
  if (newMode < 0 || newMode > MODE_WEATHER || modes[newMode] == nullptr) {
    Serial.println("Invalid mode requested");
    return;
  }
  
  // Exit current mode
  if (currentMode != nullptr) {
    currentMode->exit();
  }
  
  // Switch to new mode
  currentModeType = newMode;
  currentMode = modes[newMode];
  
  // Enter new mode
  if (currentMode != nullptr) {
    currentMode->enter();
    Serial.print("Switched to mode: ");
    Serial.println(currentMode->getName());
  }
}

String ModeManager::getCurrentModeName() {
  if (currentMode != nullptr) {
    return currentMode->getName();
  }
  return "None";
}