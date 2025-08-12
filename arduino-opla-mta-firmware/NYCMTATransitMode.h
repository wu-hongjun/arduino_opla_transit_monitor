#ifndef NYC_MTA_TRANSIT_MODE_H
#define NYC_MTA_TRANSIT_MODE_H

#include "BaseMode.h"
#include "MTAManager.h"

enum TransitState {
  TRANSIT_UPTOWN = 1,
  TRANSIT_DOWNTOWN = 2
};

class NYCMTATransitMode : public BaseMode {
private:
  MTAManager* mtaManager;
  TransitState currentState;
  String stationId;
  
public:
  NYCMTATransitMode(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr);
  
  void enter() override;
  void update() override;
  void exit() override;
  void handleButtonPress(int buttonIndex) override;
  String getName() override { return "NYC MTA Transit"; }
  
private:
  void displayTransit();
  void drawRadialTransitDisplay();
  void updateTransitState();
};

#endif