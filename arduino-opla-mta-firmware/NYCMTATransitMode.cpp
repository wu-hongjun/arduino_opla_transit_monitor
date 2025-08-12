#include "NYCMTATransitMode.h"

NYCMTATransitMode::NYCMTATransitMode(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr) 
  : BaseMode(carrierPtr), mtaManager(mtaPtr) {
  currentState = TRANSIT_UPTOWN;
  stationId = "B06"; // Roosevelt Island - F Train
}

void NYCMTATransitMode::enter() {
  Serial.println("Entering NYC MTA Transit Mode");
  // Fetch fresh MTA data
  mtaManager->updateStationData(stationId.c_str());
  displayTransit();
}

void NYCMTATransitMode::update() {
  // Update display if needed (could check for data changes)
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  
  // Refresh display every 30 seconds
  if (now - lastUpdate > 30000) {
    displayTransit();
    lastUpdate = now;
  }
}

void NYCMTATransitMode::exit() {
  Serial.println("Exiting NYC MTA Transit Mode");
  // Clear display or perform cleanup
}

void NYCMTATransitMode::handleButtonPress(int buttonIndex) {
  if (buttonIndex == 1) { // TOUCH1 - cycle between uptown/downtown
    updateTransitState();
    displayTransit();
  }
}

void NYCMTATransitMode::updateTransitState() {
  if (currentState == TRANSIT_UPTOWN) {
    currentState = TRANSIT_DOWNTOWN;
    Serial.println("Switching to Downtown trains");
  } else {
    currentState = TRANSIT_UPTOWN;
    Serial.println("Switching to Uptown trains");
  }
}

void NYCMTATransitMode::displayTransit() {
  StationData data = mtaManager->getStationData();
  
  if (!data.hasData) {
    Serial.println("No transit data available");
    // Display "No Data" message
    carrier->display.fillScreen(ST77XX_BLACK);
    carrier->display.setTextColor(ST77XX_WHITE);
    carrier->display.setTextSize(2);
    carrier->display.setCursor(50, 110);
    carrier->display.print("No Data");
    return;
  }
  
  // Use radial display for transit info
  drawRadialTransitDisplay();
}

void NYCMTATransitMode::drawRadialTransitDisplay() {
  StationData data = mtaManager->getStationData();
  const int centerX = 120;
  const int centerY = 120;
  
  // Clear screen with F train orange
  radialDisplay->clear(0xFD20);
  
  // Center: Line letter
  radialDisplay->drawCenterElement(centerX, centerY, 30, "F", ST77XX_BLACK, 0xFD20, 4);
  
  // 1st Ring: Station name
  RadialElement stationElements[1];
  stationElements[0] = radialDisplay->createTextElement(0, "Roosevelt Island", ST77XX_BLACK);
  
  RadialRing stationRing = radialDisplay->createTextRing(50, 1, ST77XX_BLACK);
  stationRing.elementCount = 1;
  stationRing.elements = stationElements;
  radialDisplay->drawRing(centerX, centerY, stationRing);
  
  // 2nd Ring: Direction indicator
  String direction = (currentState == TRANSIT_UPTOWN) ? "UPTOWN" : "DOWNTOWN";
  RadialElement directionElements[1];
  directionElements[0] = radialDisplay->createTextElement(0, direction, ST77XX_BLACK);
  
  RadialRing directionRing = radialDisplay->createTextRing(70, 2, ST77XX_BLACK);
  directionRing.elementCount = 1;
  directionRing.elements = directionElements;
  radialDisplay->drawRing(centerX, centerY, directionRing);
  
  // 3rd Ring: Train arrival times
  TrainArrival* arrivals = (currentState == TRANSIT_UPTOWN) ? data.uptown : data.downtown;
  int validCount = 0;
  RadialElement timeElements[3];
  
  for (int i = 0; i < 3 && validCount < 3; i++) {
    if (arrivals[i].isValid) {
      float angle = 90 + (validCount * 120); // 90, 210, 330 degrees
      String timeText = String(arrivals[i].minutesAway) + "m";
      timeElements[validCount] = radialDisplay->createCircleElement(angle, timeText, ST77XX_WHITE, 20);
      validCount++;
    }
  }
  
  if (validCount > 0) {
    RadialRing timeRing = radialDisplay->createCircleRing(95, 20, ST77XX_WHITE, ST77XX_BLACK);
    timeRing.elementCount = validCount;
    timeRing.elements = timeElements;
    timeRing.autoSpacing = false;
    timeRing.textSize = 2;
    radialDisplay->drawRing(centerX, centerY, timeRing);
  }
}