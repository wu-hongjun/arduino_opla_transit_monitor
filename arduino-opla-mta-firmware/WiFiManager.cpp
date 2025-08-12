#include "WiFiManager.h"

WiFiManager::WiFiManager() {
  currentNetworkIndex = 0;
  lastConnectionAttempt = 0;
  connectionTimeout = 0;
  status = WIFI_DISCONNECTED;
}

void WiFiManager::begin() {
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi Manager initialized");
  attemptConnection();
}

void WiFiManager::update() {
  if (WiFi.status() == WL_CONNECTED && status != WIFI_CONNECTED) {
    status = WIFI_CONNECTED;
    Serial.print("Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } 
  else if (WiFi.status() != WL_CONNECTED && status == WIFI_CONNECTED) {
    status = WIFI_DISCONNECTED;
    Serial.println("WiFi connection lost");
  }
  
  // Handle connection timeouts and retries
  if (status == WIFI_CONNECTING) {
    if (millis() - connectionTimeout > CONNECTION_TIMEOUT_MS) {
      Serial.println("Connection timeout, trying next network");
      nextNetwork();
    }
  }
  
  // Retry connection if disconnected and enough time has passed
  if (status == WIFI_DISCONNECTED) {
    if (millis() - lastConnectionAttempt > RETRY_DELAY_MS) {
      attemptConnection();
    }
  }
}

bool WiFiManager::isConnected() {
  return status == WIFI_CONNECTED;
}

bool WiFiManager::isConnecting() {
  return status == WIFI_CONNECTING;
}

WiFiConnectionStatus WiFiManager::getStatus() {
  return status;
}

void WiFiManager::attemptConnection() {
  // Find next valid network
  while (currentNetworkIndex < 5 && strlen(WIFI_NETWORKS[currentNetworkIndex].ssid) == 0) {
    currentNetworkIndex++;
  }
  
  // If we've tried all networks, start over
  if (currentNetworkIndex >= 5) {
    currentNetworkIndex = 0;
    lastConnectionAttempt = millis();
    return;
  }
  
  const WiFiCredentials& network = WIFI_NETWORKS[currentNetworkIndex];
  if (strlen(network.ssid) > 0) {
    Serial.print("Attempting to connect to: ");
    Serial.println(network.ssid);
    
    WiFi.begin(network.ssid, network.password);
    status = WIFI_CONNECTING;
    connectionTimeout = millis();
  }
}

void WiFiManager::nextNetwork() {
  currentNetworkIndex++;
  attemptConnection();
}