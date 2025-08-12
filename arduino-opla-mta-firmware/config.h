/*
 * Configuration file for Arduino Opla MTA Firmware
 * Modify these values for your specific setup
 */

#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration - Add your networks in priority order
struct WiFiCredentials {
  const char* ssid;
  const char* password;
  int priority;
};

const WiFiCredentials WIFI_NETWORKS[] = {
  {"La Rivière", "DoReMiMe0w!", 1},
  {"YourOfficeWiFi", "your_office_password", 2},
  {"YourMobileHotspot", "hotspot_password", 3},
  {"", "", 0}, // End marker
  {"", "", 0}  // End marker
};

// MTA Configuration - Configure your train/station combinations
struct MTAConfig {
  const char* trainLine;
  const char* stationId;
  const char* stationName;
};

const MTAConfig MTA_CONFIGS[3] = {
  {"4", "401N", "Union Sq - 14 St"},     // Button 1
  {"L", "L08N", "14 St - Union Sq"},     // Button 2  
  {"6", "626N", "Astor Pl"}              // Button 3
};

// Weather Configuration
const float WEATHER_LATITUDE = 40.7589;   // NYC coordinates
const float WEATHER_LONGITUDE = -73.9851;

// API Keys - Replace with your actual keys
const char* MTA_API_KEY = "your_mta_api_key_here";
const char* WEATHER_API_KEY = "your_openweather_api_key_here";

// Refresh Intervals (in seconds)
const int MTA_REFRESH_INTERVAL = 120;     // 2 minutes
const int WEATHER_REFRESH_INTERVAL = 600; // 10 minutes
const int RAIN_REFRESH_INTERVAL = 900;    // 15 minutes

// Hardware Pin Definitions (Arduino Opla specific)
const int BUTTON_PINS[5] = {0, 1, 2, 3, 4}; // Update with actual pin numbers
const int LED_L1_PIN = 25;  // Update with actual pin number
const int LED_L2_PIN = 26;  // Update with actual pin number

#endif