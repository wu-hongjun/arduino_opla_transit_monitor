#include "RadialDisplay.h"
#include <math.h>

RadialDisplay::RadialDisplay(MKRIoTCarrier* carrierPtr) {
  carrier = carrierPtr;
}

void RadialDisplay::clear(uint16_t backgroundColor) {
  carrier->display.fillScreen(backgroundColor);
}

void RadialDisplay::calculatePosition(int centerX, int centerY, int radius, float angle, int& x, int& y) {
  float radians = (angle - 90) * PI / 180.0; // -90 to start from top (0°)
  x = centerX + (radius * cos(radians));
  y = centerY + (radius * sin(radians));
}

void RadialDisplay::drawRadialLayout(RadialDisplayConfig& config) {
  // Clear screen
  clear(config.backgroundColor);
  
  // Draw each ring from outermost to innermost
  for (int i = config.ringCount - 1; i >= 0; i--) {
    drawRing(config.centerX, config.centerY, config.rings[i]);
  }
}

void RadialDisplay::drawRing(int centerX, int centerY, RadialRing& ring) {
  // Draw ring background first
  drawRingBackground(centerX, centerY, ring);
  
  // Draw ring content based on type
  switch (ring.type) {
    case RadialRing::RING_TEXT_CIRCULAR:
      drawTextRing(centerX, centerY, ring);
      break;
    case RadialRing::RING_CIRCLES:
      drawCircleRing(centerX, centerY, ring);
      break;
    case RadialRing::RING_ARCS:
      drawArcRing(centerX, centerY, ring);
      break;
    case RadialRing::RING_DOTS:
      drawDotRing(centerX, centerY, ring);
      break;
    case RadialRing::RING_BACKGROUND:
      // Already drawn background, nothing more needed
      break;
  }
}

void RadialDisplay::drawRingBackground(int centerX, int centerY, RadialRing& ring) {
  if (ring.thickness > 0) {
    // Draw filled ring background
    carrier->display.fillCircle(centerX, centerY, ring.radius + ring.thickness/2, ring.bgColor);
    if (ring.radius > ring.thickness/2) {
      carrier->display.fillCircle(centerX, centerY, ring.radius - ring.thickness/2, 0x0000); // Cut out center
    }
  }
  
  if (ring.borderWidth > 0) {
    // Draw border
    for (int i = 0; i < ring.borderWidth; i++) {
      carrier->display.drawCircle(centerX, centerY, ring.radius + i, ring.borderColor);
    }
  }
}

void RadialDisplay::drawTextRing(int centerX, int centerY, RadialRing& ring) {
  carrier->display.setTextSize(ring.textSize);
  
  float angleRange = ring.endAngle - ring.startAngle;
  if (angleRange <= 0) angleRange = 360; // Full circle
  
  for (int i = 0; i < ring.elementCount; i++) {
    if (!ring.elements[i].isVisible) continue;
    
    float angle;
    if (ring.autoSpacing) {
      // Auto-calculate angle based on position in array
      angle = ring.startAngle + (i * angleRange / ring.elementCount);
    } else {
      // Use specified angle
      angle = ring.elements[i].angle;
    }
    
    int x, y;
    calculatePosition(centerX, centerY, ring.radius, angle, x, y);
    
    carrier->display.setTextColor(ring.elements[i].color);
    
    // Center text approximately
    int textWidth = ring.elements[i].content.length() * 6 * ring.textSize;
    carrier->display.setCursor(x - textWidth/2, y - 4 * ring.textSize);
    carrier->display.print(ring.elements[i].content);
  }
}

void RadialDisplay::drawCircleRing(int centerX, int centerY, RadialRing& ring) {
  for (int i = 0; i < ring.elementCount; i++) {
    if (!ring.elements[i].isVisible) continue;
    
    float angle;
    if (ring.autoSpacing) {
      float angleRange = ring.endAngle - ring.startAngle;
      if (angleRange <= 0) angleRange = 360;
      angle = ring.startAngle + (i * angleRange / ring.elementCount);
    } else {
      angle = ring.elements[i].angle;
    }
    
    int x, y;
    calculatePosition(centerX, centerY, ring.radius, angle, x, y);
    
    int circleSize = ring.elements[i].size > 0 ? ring.elements[i].size : 15;
    
    // Draw circle
    carrier->display.fillCircle(x, y, circleSize, ring.elements[i].color);
    if (ring.borderWidth > 0) {
      carrier->display.drawCircle(x, y, circleSize, ring.borderColor);
    }
    
    // Draw content if any
    if (ring.elements[i].content.length() > 0) {
      carrier->display.setTextColor(ring.borderColor);
      carrier->display.setTextSize(ring.textSize);
      
      int textWidth = ring.elements[i].content.length() * 6 * ring.textSize;
      carrier->display.setCursor(x - textWidth/2, y - 4 * ring.textSize);
      carrier->display.print(ring.elements[i].content);
    }
  }
}

void RadialDisplay::drawArcRing(int centerX, int centerY, RadialRing& ring) {
  // Simplified arc drawing - draw as thick lines
  for (int i = 0; i < ring.elementCount; i++) {
    if (!ring.elements[i].isVisible) continue;
    
    float startAngle = ring.elements[i].angle - ring.elements[i].size/2;
    float endAngle = ring.elements[i].angle + ring.elements[i].size/2;
    
    // Draw arc as series of points
    for (float a = startAngle; a <= endAngle; a += 2) {
      int x, y;
      calculatePosition(centerX, centerY, ring.radius, a, x, y);
      carrier->display.drawPixel(x, y, ring.elements[i].color);
      
      // Make thicker
      if (ring.thickness > 1) {
        for (int t = 1; t < ring.thickness; t++) {
          int x2, y2;
          calculatePosition(centerX, centerY, ring.radius + t, a, x2, y2);
          carrier->display.drawPixel(x2, y2, ring.elements[i].color);
        }
      }
    }
  }
}

void RadialDisplay::drawDotRing(int centerX, int centerY, RadialRing& ring) {
  for (int i = 0; i < ring.elementCount; i++) {
    if (!ring.elements[i].isVisible) continue;
    
    float angle;
    if (ring.autoSpacing) {
      float angleRange = ring.endAngle - ring.startAngle;
      if (angleRange <= 0) angleRange = 360;
      angle = ring.startAngle + (i * angleRange / ring.elementCount);
    } else {
      angle = ring.elements[i].angle;
    }
    
    int x, y;
    calculatePosition(centerX, centerY, ring.radius, angle, x, y);
    
    int dotSize = ring.elements[i].size > 0 ? ring.elements[i].size : 3;
    carrier->display.fillCircle(x, y, dotSize, ring.elements[i].color);
  }
}

void RadialDisplay::drawCenterElement(int centerX, int centerY, int radius, String text, 
                                    uint16_t bgColor, uint16_t textColor, int textSize) {
  // Draw circle background
  carrier->display.fillCircle(centerX, centerY, radius, bgColor);
  carrier->display.drawCircle(centerX, centerY, radius, textColor);
  
  // Draw text in center
  carrier->display.setTextColor(textColor);
  carrier->display.setTextSize(textSize);
  
  int textWidth = text.length() * 6 * textSize;
  int textHeight = 8 * textSize;
  
  carrier->display.setCursor(centerX - textWidth/2, centerY - textHeight/2);
  carrier->display.print(text);
}

void RadialDisplay::drawSimpleRing(int centerX, int centerY, int radius, RadialElement* elements, 
                                  int count, RadialRing::RingType type) {
  RadialRing ring = {0};
  ring.radius = radius;
  ring.type = type;
  ring.elementCount = count;
  ring.elements = elements;
  ring.autoSpacing = true;
  ring.startAngle = 0;
  ring.endAngle = 360;
  ring.textSize = 1;
  
  drawRing(centerX, centerY, ring);
}

// Utility functions
RadialElement RadialDisplay::createTextElement(float angle, String text, uint16_t color) {
  RadialElement element = {0};
  element.angle = angle;
  element.content = text;
  element.color = color;
  element.isVisible = true;
  element.size = 0;
  return element;
}

RadialElement RadialDisplay::createCircleElement(float angle, String content, uint16_t color, int size) {
  RadialElement element = {0};
  element.angle = angle;
  element.content = content;
  element.color = color;
  element.isVisible = true;
  element.size = size;
  return element;
}

RadialRing RadialDisplay::createTextRing(int radius, int textSize, uint16_t color) {
  RadialRing ring = {0};
  ring.radius = radius;
  ring.type = RadialRing::RING_TEXT_CIRCULAR;
  ring.textSize = textSize;
  ring.autoSpacing = true;
  ring.startAngle = 0;
  ring.endAngle = 360;
  return ring;
}

RadialRing RadialDisplay::createCircleRing(int radius, int circleSize, uint16_t fillColor, uint16_t borderColor) {
  RadialRing ring = {0};
  ring.radius = radius;
  ring.type = RadialRing::RING_CIRCLES;
  ring.bgColor = fillColor;
  ring.borderColor = borderColor;
  ring.borderWidth = 1;
  ring.autoSpacing = true;
  ring.startAngle = 0;
  ring.endAngle = 360;
  return ring;
}