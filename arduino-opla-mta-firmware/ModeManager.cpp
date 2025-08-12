#include "ModeManager.h"

ModeManager::ModeManager(MKRIoTCarrier* carrierPtr, MTAManager* mtaPtr) {
  carrier = carrierPtr;
  mtaManager = mtaPtr;
  radialDisplay = new RadialDisplay(carrierPtr);
  currentMode = MODE_NONE;
  currentState = 1;
  currentTheme = THEME_LIGHT;
}

void ModeManager::begin() {
  Serial.println("Mode Manager initialized with Arduino_OplaUI");
  
  // Initialize gauges
  initializeGauges();
  
  // Clear display initially - use proper Opla UI method
  tempGauge.clear();
  humidityGauge.clear();
  
  Serial.println("Opla UI gauges initialized");
}

void ModeManager::initializeGauges() {
  // Initialize temperature gauge
  tempGauge.begin(carrier->display);
  tempGauge.setTitle("TEMPERATURE");
  tempGauge.setSuffix(" C");
  tempGauge.setRange(-10, 50);
  tempGauge.setSteps(6);
  tempGauge.setDigits(1);
  tempGauge.setBackgroundColor(0, 0, 0);
  
  // Initialize humidity gauge  
  humidityGauge.begin(carrier->display);
  humidityGauge.setTitle("HUMIDITY");
  humidityGauge.setSuffix(" %");
  humidityGauge.setRange(0, 100);
  humidityGauge.setSteps(10);
  humidityGauge.setDigits(1);
  humidityGauge.setBackgroundColor(0, 0, 0);
}

void ModeManager::update() {
  // Update theme based on light sensor
  updateTheme();
  
  // Update current mode display if needed
  if (currentMode == MODE_TEMP_HUMIDITY) {
    displayWeather(); // Renamed to show all weather data
  } else if (currentMode == MODE_TRANSIT) {
    displayTransit();
  }
}

void ModeManager::handleButtonPress(int buttonIndex) {
  Serial.print("Button pressed: ");
  Serial.println(buttonIndex);
  
  switch (buttonIndex) {
    case 0: // TOUCH0 - Weather mode (temp, humidity, pressure, light)
      if (currentMode == MODE_TEMP_HUMIDITY) {
        // Already in weather mode, cycle state
        updateWeatherState();
      } else {
        // Enter weather mode
        currentMode = MODE_TEMP_HUMIDITY;
        currentState = TEMP_CELSIUS;
        Serial.println("Entering Weather Mode - Celsius");
        displayWeather();
      }
      break;
      
    case 1: // TOUCH1 - Transit mode
      if (currentMode == MODE_TRANSIT) {
        // Already in transit mode, cycle state
        updateTransitState();
      } else {
        // Enter transit mode
        currentMode = MODE_TRANSIT;
        currentState = TRANSIT_UPTOWN;
        Serial.println("Entering Transit Mode - Uptown");
        // Fetch fresh MTA data
        mtaManager->updateStationData("B06"); // Roosevelt Island
        displayTransit();
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

void ModeManager::updateWeatherState() {
  if (currentState == TEMP_CELSIUS) {
    currentState = TEMP_FAHRENHEIT;
    tempGauge.setSuffix(" F");
    tempGauge.setRange(14, 122); // -10C to 50C in Fahrenheit
    Serial.println("Switching to Fahrenheit");
  } else {
    currentState = TEMP_CELSIUS;
    tempGauge.setSuffix(" C");
    tempGauge.setRange(-10, 50);
    Serial.println("Switching to Celsius");
  }
  displayWeather();
}

void ModeManager::updateTheme() {
  DisplayTheme newTheme = getThemeFromLightSensor();
  if (newTheme != currentTheme) {
    currentTheme = newTheme;
    applyThemeToGauges();
    Serial.print("Theme changed to: ");
    Serial.println(currentTheme == THEME_LIGHT ? "Light" : "Dark");
  }
}

DisplayTheme ModeManager::getThemeFromLightSensor() {
  // Read light sensor
  while (!carrier->Light.colorAvailable()) {
    delay(5);
  }
  int none;
  int lightLevel;
  carrier->Light.readColor(none, none, none, lightLevel);
  
  // Threshold for switching themes (adjust as needed)
  const int LIGHT_THRESHOLD = 300;
  
  return (lightLevel > LIGHT_THRESHOLD) ? THEME_LIGHT : THEME_DARK;
}

void ModeManager::applyThemeToGauges() {
  if (currentTheme == THEME_LIGHT) {
    // Light theme - dark text on light background
    tempGauge.setBackgroundColor(255, 255, 255);
    humidityGauge.setBackgroundColor(255, 255, 255);
  } else {
    // Dark theme - light text on dark background
    tempGauge.setBackgroundColor(0, 0, 0);
    humidityGauge.setBackgroundColor(0, 0, 0);
  }
}

void ModeManager::displayWeather() {
  // Read all sensors
  float temperature = carrier->Env.readTemperature();
  float humidity = carrier->Env.readHumidity();
  float pressure = carrier->Pressure.readPressure();
  
  // Read light sensor
  while (!carrier->Light.colorAvailable()) {
    delay(5);
  }
  int none;
  int lightLevel;
  carrier->Light.readColor(none, none, none, lightLevel);
  
  // Convert temperature if needed
  float displayTemp = temperature;
  String tempUnit = "C";
  if (currentState == TEMP_FAHRENHEIT) {
    displayTemp = (temperature * 9.0 / 5.0) + 32.0;
    tempUnit = "F";
  }
  
  // Only redraw if values changed significantly (avoid constant redraws)
  static float lastDisplayTemp = -999;
  static float lastHumidity = -999;
  static int lastLightLevel = -999;
  static bool firstDraw = true;
  
  bool tempChanged = abs(displayTemp - lastDisplayTemp) > 0.1;
  bool humidityChanged = abs(humidity - lastHumidity) > 0.5;
  bool lightChanged = abs(lightLevel - lastLightLevel) > 50;
  
  if (firstDraw || tempChanged || humidityChanged || lightChanged) {
    drawRadialWeatherDisplay(displayTemp, tempUnit, humidity, lightLevel);
    
    Serial.print("Weather - Temp: ");
    Serial.print(displayTemp);
    Serial.print(" ");
    Serial.print(tempUnit);
    Serial.print(", Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Light: ");
    Serial.print(lightLevel);
    Serial.println(" lux");
    
    lastDisplayTemp = displayTemp;
    lastHumidity = humidity;
    lastLightLevel = lightLevel;
    firstDraw = false;
  }
}

DisplayMode ModeManager::getCurrentMode() {
  return currentMode;
}

void ModeManager::updateTransitState() {
  if (currentState == TRANSIT_UPTOWN) {
    currentState = TRANSIT_DOWNTOWN;
    Serial.println("Switching to Downtown trains");
  } else {
    currentState = TRANSIT_UPTOWN;
    Serial.println("Switching to Uptown trains");
  }
  displayTransit();
}

void ModeManager::displayTransit() {
  StationData data = mtaManager->getStationData();
  
  if (!data.hasData) {
    Serial.println("No transit data available");
    return;
  }
  
  // Simplified transit display for now
  carrier->display.fillScreen(0xFD20); // F train orange
  carrier->display.setTextColor(ST77XX_BLACK);
  carrier->display.setTextSize(3);
  carrier->display.setCursor(60, 100);
  carrier->display.print("F TRAIN");
  
  carrier->display.setTextSize(1);
  carrier->display.setCursor(30, 140);
  carrier->display.print("Roosevelt Island");
  
  // Show first uptown and downtown times
  if (data.uptown[0].isValid) {
    carrier->display.setCursor(30, 160);
    carrier->display.print("Up: ");
    carrier->display.print(data.uptown[0].minutesAway);
    carrier->display.print(" min");
  }
  
  if (data.downtown[0].isValid) {
    carrier->display.setCursor(30, 180);
    carrier->display.print("Down: ");
    carrier->display.print(data.downtown[0].minutesAway);
    carrier->display.print(" min");
  }
  
  // Log the data
  Serial.print("Transit - Uptown: ");
  for (int i = 0; i < 3; i++) {
    if (data.uptown[i].isValid) {
      Serial.print(data.uptown[i].minutesAway);
      if (i < 2) Serial.print(", ");
    }
  }
  Serial.print(" | Downtown: ");
  for (int i = 0; i < 3; i++) {
    if (data.downtown[i].isValid) {
      Serial.print(data.downtown[i].minutesAway);
      if (i < 2) Serial.print(", ");
    }
  }
  Serial.println();
}

DisplayMode ModeManager::getCurrentMode() {
  return currentMode;
}

int ModeManager::getCurrentState() {
  return currentState;
}
void ModeManager::drawRadialWeatherDisplay(float temperature, String tempUnit, float humidity, int lightLevel) {
  const int centerX = 120;
  const int centerY = 120;
  
  // Theme colors
  uint16_t bgColor = (currentTheme == THEME_LIGHT) ? ST77XX_WHITE : ST77XX_BLACK;
  uint16_t textColor = (currentTheme == THEME_LIGHT) ? ST77XX_BLACK : ST77XX_WHITE;
  uint16_t accentColor = (currentTheme == THEME_LIGHT) ? 0x7BEF : ST77XX_YELLOW; // Light blue or yellow
  
  // Clear screen
  radialDisplay->clear(bgColor);
  
  // Center: Sun/Moon icon based on theme
  String centerIcon = (currentTheme == THEME_LIGHT) ? "☀" : "☽";
  radialDisplay->drawCenterElement(centerX, centerY, 20, centerIcon, accentColor, textColor, 3);
  
  // 1st Ring: Light intensity label and value
  RadialElement lightElements[2];
  lightElements[0] = radialDisplay->createTextElement(270, "LIGHT", textColor); // Left side
  lightElements[1] = radialDisplay->createTextElement(90, String(lightLevel), accentColor); // Right side
  
  RadialRing lightRing = radialDisplay->createTextRing(45, 1, textColor);
  lightRing.elementCount = 2;
  lightRing.elements = lightElements;
  lightRing.autoSpacing = false; // Use specified angles
  radialDisplay->drawRing(centerX, centerY, lightRing);
  
  // 2nd Ring: Temperature (top) and Humidity (bottom) values
  RadialElement valueElements[2];
  valueElements[0] = radialDisplay->createCircleElement(0, String((int)temperature) + tempUnit, accentColor, 18); // Top
  valueElements[1] = radialDisplay->createCircleElement(180, String((int)humidity) + "%", accentColor, 18); // Bottom
  
  RadialRing valueRing = radialDisplay->createCircleRing(70, 18, accentColor, textColor);
  valueRing.elementCount = 2;
  valueRing.elements = valueElements;
  valueRing.autoSpacing = false; // Use specified angles
  valueRing.textSize = 1;
  radialDisplay->drawRing(centerX, centerY, valueRing);
  
  // 3rd Ring: Temperature and Humidity labels
  RadialElement labelElements[2];
  labelElements[0] = radialDisplay->createTextElement(0, "TEMP", textColor); // Top
  labelElements[1] = radialDisplay->createTextElement(180, "HUMID", textColor); // Bottom
  
  RadialRing labelRing = radialDisplay->createTextRing(95, 1, textColor);
  labelRing.elementCount = 2;
  labelRing.elements = labelElements;
  labelRing.autoSpacing = false; // Use specified angles
  radialDisplay->drawRing(centerX, centerY, labelRing);
}