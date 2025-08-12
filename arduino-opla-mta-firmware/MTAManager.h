/*
 * MTA Manager for Arduino Opla MTA Firmware
 * Handles NYC MTA real-time transit data
 */

#ifndef MTAMANAGER_H
#define MTAMANAGER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "config.h"

struct TrainArrival {
  String route;
  String destination;
  int minutesAway;
  bool isValid;
};

struct StationData {
  TrainArrival uptown[3];    // Next 3 uptown trains
  TrainArrival downtown[3];  // Next 3 downtown trains
  unsigned long lastUpdate;
  bool hasData;
};

class MTAManager {
private:
  WiFiClient wifiClient;
  HttpClient* httpClient;
  StationData stationData;
  
  // MTA API endpoints (we'll use a simplified proxy service)
  const char* MTA_PROXY_HOST = "api.example.com"; // Replace with actual proxy
  const int MTA_PROXY_PORT = 80;
  
  bool fetchStationData(const char* stationId);
  void parseTrainData(String jsonResponse);
  void clearStationData();

public:
  MTAManager();
  void begin();
  bool updateStationData(const char* stationId);
  StationData getStationData();
  bool hasValidData();
  unsigned long getLastUpdateTime();
};

#endif