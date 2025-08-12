#include "AmbientDataMode.h"

AmbientDataMode::AmbientDataMode(MKRIoTCarrier* carrierPtr) : BaseMode(carrierPtr) {
  currentState = TEMP_CELSIUS;
  currentTheme = THEME_LIGHT;
}

void AmbientDataMode::enter() {
  Serial.println("Entering Ambient Data Mode");
  displayWeather();
}

void AmbientDataMode::update() {
  // Update theme based on light sensor
  updateTheme();
  
  // Update display
  displayWeather();
}

void AmbientDataMode::exit() {
  Serial.println("Exiting Ambient Data Mode");
  // Clear display or perform cleanup
}

void AmbientDataMode::handleButtonPress(int buttonIndex) {
  if (buttonIndex == 0) { // TOUCH0 - toggle temperature units
    updateWeatherState();
    displayWeather();
  }
}

void AmbientDataMode::updateWeatherState() {
  if (currentState == TEMP_CELSIUS) {
    currentState = TEMP_FAHRENHEIT;
    Serial.println("Switching to Fahrenheit");
  } else {
    currentState = TEMP_CELSIUS;
    Serial.println("Switching to Celsius");
  }
}

void AmbientDataMode::updateTheme() {
  DisplayTheme newTheme = getThemeFromLightSensor();
  if (newTheme != currentTheme) {
    currentTheme = newTheme;
    Serial.print("Theme changed to: ");
    Serial.println(currentTheme == THEME_LIGHT ? "Light" : "Dark");
  }
}

DisplayTheme AmbientDataMode::getThemeFromLightSensor() {
  // Read light sensor
  while (!carrier->Light.colorAvailable()) {
    delay(5);
  }
  int none;
  int lightLevel;
  carrier->Light.readColor(none, none, none, lightLevel);
  
  // Threshold for switching themes
  const int LIGHT_THRESHOLD = 300;
  
  return (lightLevel > LIGHT_THRESHOLD) ? THEME_LIGHT : THEME_DARK;
}

void AmbientDataMode::displayWeather() {
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
  
  // Only redraw if values changed significantly
  static float lastDisplayTemp = -999;
  static float lastHumidity = -999;
  static int lastLightLevel = -999;
  static bool firstDraw = true;
  
  bool tempChanged = abs(displayTemp - lastDisplayTemp) > 0.1;
  bool humidityChanged = abs(humidity - lastHumidity) > 0.5;
  bool lightChanged = abs(lightLevel - lastLightLevel) > 50;
  
  if (firstDraw || tempChanged || humidityChanged || lightChanged) {
    drawRadialWeatherDisplay(displayTemp, tempUnit, humidity, lightLevel);
    
    Serial.print("Ambient - Temp: ");
    Serial.print(displayTemp);
    Serial.print(" ");
    Serial.print(tempUnit);
    Serial.print(", Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Light: ");
    Serial.print(lightLevel);
    Serial.print(" lux, Pressure: ");
    Serial.print(pressure);
    Serial.println(" kPa");
    
    lastDisplayTemp = displayTemp;
    lastHumidity = humidity;
    lastLightLevel = lightLevel;
    firstDraw = false;
  }
}

void AmbientDataMode::drawRadialWeatherDisplay(float temperature, String tempUnit, float humidity, int lightLevel) {
  const int centerX = 120;
  const int centerY = 120;
  
  // Theme colors
  uint16_t bgColor = (currentTheme == THEME_LIGHT) ? ST77XX_WHITE : ST77XX_BLACK;
  uint16_t textColor = (currentTheme == THEME_LIGHT) ? ST77XX_BLACK : ST77XX_WHITE;
  uint16_t accentColor = (currentTheme == THEME_LIGHT) ? 0x7BEF : ST77XX_YELLOW;
  
  // Clear screen
  radialDisplay->clear(bgColor);
  
  // Center: Sun/Moon icon based on theme
  String centerIcon = (currentTheme == THEME_LIGHT) ? "☀" : "☽";
  radialDisplay->drawCenterElement(centerX, centerY, 20, centerIcon, accentColor, textColor, 3);
  
  // 1st Ring: Light intensity label and value
  RadialElement lightElements[2];
  lightElements[0] = radialDisplay->createTextElement(270, "LIGHT", textColor);
  lightElements[1] = radialDisplay->createTextElement(90, String(lightLevel), accentColor);
  
  RadialRing lightRing = radialDisplay->createTextRing(45, 1, textColor);
  lightRing.elementCount = 2;
  lightRing.elements = lightElements;
  lightRing.autoSpacing = false;
  radialDisplay->drawRing(centerX, centerY, lightRing);
  
  // 2nd Ring: Temperature (top) and Humidity (bottom) values
  RadialElement valueElements[2];
  valueElements[0] = radialDisplay->createCircleElement(0, String((int)temperature) + tempUnit, accentColor, 18);
  valueElements[1] = radialDisplay->createCircleElement(180, String((int)humidity) + "%", accentColor, 18);
  
  RadialRing valueRing = radialDisplay->createCircleRing(70, 18, accentColor, textColor);
  valueRing.elementCount = 2;
  valueRing.elements = valueElements;
  valueRing.autoSpacing = false;
  valueRing.textSize = 1;
  radialDisplay->drawRing(centerX, centerY, valueRing);
  
  // 3rd Ring: Temperature and Humidity labels
  RadialElement labelElements[2];
  labelElements[0] = radialDisplay->createTextElement(0, "TEMP", textColor);
  labelElements[1] = radialDisplay->createTextElement(180, "HUMID", textColor);
  
  RadialRing labelRing = radialDisplay->createTextRing(95, 1, textColor);
  labelRing.elementCount = 2;
  labelRing.elements = labelElements;
  labelRing.autoSpacing = false;
  radialDisplay->drawRing(centerX, centerY, labelRing);
}