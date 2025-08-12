#include "MTAManager.h"
#include <ArduinoJson.h>

MTAManager::MTAManager() {
  httpClient = nullptr;
  clearStationData();
}

void MTAManager::begin() {
  // Initialize HTTP client
  httpClient = new HttpClient(wifiClient, MTA_PROXY_HOST, MTA_PROXY_PORT);
  Serial.println("MTA Manager initialized");
}

void MTAManager::clearStationData() {
  stationData.hasData = false;
  stationData.lastUpdate = 0;
  
  // Clear all train data
  for (int i = 0; i < 3; i++) {
    stationData.uptown[i].isValid = false;
    stationData.downtown[i].isValid = false;
  }
}

bool MTAManager::updateStationData(const char* stationId) {
  if (!httpClient) {
    Serial.println("HTTP client not initialized");
    return false;
  }
  
  Serial.print("Fetching MTA data for station: ");
  Serial.println(stationId);
  
  // For now, simulate MTA data since we need a proxy service
  // In production, this would call: /api/mta/station/{stationId}
  
  // Simulate Roosevelt Island F train data
  clearStationData();
  
  // Simulate uptown (Queens-bound) F trains
  stationData.uptown[0] = {"F", "179 St", 2, true};
  stationData.uptown[1] = {"F", "179 St", 8, true};
  stationData.uptown[2] = {"F", "179 St", 15, true};
  
  // Simulate downtown (Manhattan-bound) F trains
  stationData.downtown[0] = {"F", "Coney Island", 4, true};
  stationData.downtown[1] = {"F", "Coney Island", 11, true};
  stationData.downtown[2] = {"F", "Coney Island", 18, true};
  
  stationData.hasData = true;
  stationData.lastUpdate = millis();
  
  Serial.println("MTA data updated (simulated)");
  return true;
  
  /* 
  // Real implementation would look like this:
  String endpoint = "/api/mta/station/";
  endpoint += stationId;
  
  httpClient->beginRequest();
  httpClient->get(endpoint);
  httpClient->endRequest();
  
  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();
  
  if (statusCode == 200) {
    parseTrainData(response);
    stationData.lastUpdate = millis();
    return true;
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(statusCode);
    return false;
  }
  */
}

void MTAManager::parseTrainData(String jsonResponse) {
  // Parse JSON response from MTA proxy service
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, jsonResponse);
  
  clearStationData();
  
  // Parse uptown trains
  JsonArray uptown = doc["uptown"];
  for (int i = 0; i < 3 && i < uptown.size(); i++) {
    stationData.uptown[i].route = uptown[i]["route"].as<String>();
    stationData.uptown[i].destination = uptown[i]["destination"].as<String>();
    stationData.uptown[i].minutesAway = uptown[i]["minutes"].as<int>();
    stationData.uptown[i].isValid = true;
  }
  
  // Parse downtown trains
  JsonArray downtown = doc["downtown"];
  for (int i = 0; i < 3 && i < downtown.size(); i++) {
    stationData.downtown[i].route = downtown[i]["route"].as<String>();
    stationData.downtown[i].destination = downtown[i]["destination"].as<String>();
    stationData.downtown[i].minutesAway = downtown[i]["minutes"].as<int>();
    stationData.downtown[i].isValid = true;
  }
  
  stationData.hasData = true;
}

StationData MTAManager::getStationData() {
  return stationData;
}

bool MTAManager::hasValidData() {
  return stationData.hasData && (millis() - stationData.lastUpdate < 300000); // 5 minutes
}

unsigned long MTAManager::getLastUpdateTime() {
  return stationData.lastUpdate;
}