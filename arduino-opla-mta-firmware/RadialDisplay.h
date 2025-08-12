/*
 * Generic Radial Display Library for Arduino Opla
 * Flexible library for creating customizable circular/radial UI layouts
 */

#ifndef RADIALDISPLAY_H
#define RADIALDISPLAY_H

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>

// Generic radial element that can hold any type of content
struct RadialElement {
  float angle;          // Position angle in degrees (0° = top, clockwise)
  String content;       // Text content to display
  uint16_t color;       // Element color
  bool isVisible;       // Whether to show this element
  int size;            // Size parameter (context-dependent)
};

// Configuration for a radial ring
struct RadialRing {
  int radius;           // Distance from center
  int thickness;        // Ring thickness (for backgrounds/borders)
  uint16_t bgColor;     // Background color
  uint16_t borderColor; // Border color
  int borderWidth;      // Border thickness
  int textSize;         // Text size for elements in this ring
  int elementCount;     // Number of elements in this ring
  RadialElement* elements; // Array of elements
  
  // Ring type and layout
  enum RingType {
    RING_TEXT_CIRCULAR,   // Text arranged in a circle
    RING_CIRCLES,         // Individual circles with content
    RING_ARCS,           // Arc segments
    RING_DOTS,           // Small dots/indicators
    RING_BACKGROUND      // Just a background ring
  } type;
  
  // Layout options
  float startAngle;     // Starting angle for first element
  float endAngle;       // Ending angle for last element (for partial rings)
  bool autoSpacing;     // Auto-calculate spacing between elements
};

// Main radial display configuration
struct RadialDisplayConfig {
  int centerX, centerY; // Center point
  uint16_t backgroundColor; // Screen background
  int ringCount;        // Number of rings
  RadialRing* rings;    // Array of ring configurations
};

class RadialDisplay {
private:
  MKRIoTCarrier* carrier;
  
  // Helper functions
  void calculatePosition(int centerX, int centerY, int radius, float angle, int& x, int& y);
  void drawRingBackground(int centerX, int centerY, RadialRing& ring);
  void drawTextRing(int centerX, int centerY, RadialRing& ring);
  void drawCircleRing(int centerX, int centerY, RadialRing& ring);
  void drawArcRing(int centerX, int centerY, RadialRing& ring);
  void drawDotRing(int centerX, int centerY, RadialRing& ring);

public:
  RadialDisplay(MKRIoTCarrier* carrierPtr);
  
  // Core functions
  void clear(uint16_t backgroundColor = 0x0000);
  void drawRadialLayout(RadialDisplayConfig& config);
  void drawRing(int centerX, int centerY, RadialRing& ring);
  
  // Convenience functions for common patterns
  void drawCenterElement(int centerX, int centerY, int radius, String text, 
                        uint16_t bgColor, uint16_t textColor, int textSize);
  void drawSimpleRing(int centerX, int centerY, int radius, RadialElement* elements, 
                     int count, RadialRing::RingType type);
  
  // Utility functions
  RadialElement createTextElement(float angle, String text, uint16_t color);
  RadialElement createCircleElement(float angle, String content, uint16_t color, int size);
  RadialRing createTextRing(int radius, int textSize, uint16_t color);
  RadialRing createCircleRing(int radius, int circleSize, uint16_t fillColor, uint16_t borderColor);
};

#endif