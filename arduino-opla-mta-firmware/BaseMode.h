#ifndef BASE_MODE_H
#define BASE_MODE_H

#include <Arduino_MKRIoTCarrier.h>
#include "RadialDisplay.h"

class BaseMode {
protected:
  MKRIoTCarrier* carrier;
  RadialDisplay* radialDisplay;
  
public:
  BaseMode(MKRIoTCarrier* carrierPtr) : carrier(carrierPtr) {
    radialDisplay = new RadialDisplay(carrierPtr);
  }
  
  virtual ~BaseMode() {
    delete radialDisplay;
  }
  
  virtual void enter() = 0;
  virtual void update() = 0;
  virtual void exit() = 0;
  virtual void handleButtonPress(int buttonIndex) = 0;
  virtual String getName() = 0;
};

#endif