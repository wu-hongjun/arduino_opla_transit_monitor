#ifndef AMBIENT_DATA_MODE_H
#define AMBIENT_DATA_MODE_H

#include "BaseMode.h"

enum AmbientState {
  TEMP_CELSIUS = 1,
  TEMP_FAHRENHEIT = 2
};

enum DisplayTheme {
  THEME_LIGHT,
  THEME_DARK
};

class AmbientDataMode : public BaseMode {
private:
  AmbientState currentState;
  DisplayTheme currentTheme;
  
public:
  AmbientDataMode(MKRIoTCarrier* carrierPtr);
  
  void enter() override;
  void update() override;
  void exit() override;
  void handleButtonPress(int buttonIndex) override;
  String getName() override { return "Ambient Data"; }
  
private:
  void displayWeather();
  void drawRadialWeatherDisplay(float temperature, String tempUnit, float humidity, int lightLevel);
  void updateWeatherState();
  void updateTheme();
  DisplayTheme getThemeFromLightSensor();
};

#endif