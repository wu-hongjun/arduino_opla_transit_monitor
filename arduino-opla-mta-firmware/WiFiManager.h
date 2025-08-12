/*
 * WiFi Manager for Arduino Opla MTA Firmware
 * Handles automatic connection with failover support
 */

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFiNINA.h>
#include "config.h"

enum WiFiConnectionStatus {
  WIFI_DISCONNECTED,
  WIFI_CONNECTING,
  WIFI_CONNECTED
};

class WiFiManager {
private:
  int currentNetworkIndex;
  unsigned long lastConnectionAttempt;
  unsigned long connectionTimeout;
  WiFiConnectionStatus status;
  
  static const unsigned long CONNECTION_TIMEOUT_MS = 10000; // 10 seconds
  static const unsigned long RETRY_DELAY_MS = 30000;       // 30 seconds

public:
  WiFiManager();
  void begin();
  void update();
  bool isConnected();
  bool isConnecting();
  WiFiConnectionStatus getStatus();
  void attemptConnection();
  void nextNetwork();
};

#endif