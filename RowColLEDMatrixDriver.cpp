#ifndef _RowColLEDMatrixDriver_CPP_
#define _RowColLEDMatrixDriver_CPP_

#include "RowColLEDMatrixDriver.hpp"


int RowColLEDMatrixDriver::getPixelSize() {
  return PIXELS;
}

int RowColLEDMatrixDriver::getMatrixWidth() {
  return W * PIXELS;
}

int RowColLEDMatrixDriver::getMatrixHeight() {
  return H * PIXELS;
}

RowColLEDMatrixDriver::RowColLEDMatrixDriver(uint8_t W, uint8_t H, uint8_t ssPin, uint8_t PIXELS):
  LEDMatrixDriver(W * H, ssPin),
  W(W),
  H(H),
  PIXELS(PIXELS)
{
  setIntensity(10);
}

void RowColLEDMatrixDriver::setEnabled(bool enabled) {
  LEDMatrixDriver::setEnabled(enabled);
}
void RowColLEDMatrixDriver::setIntensity(uint8_t level) {
  intensity = level;
  LEDMatrixDriver::setIntensity(level);
}
void RowColLEDMatrixDriver::display() {
  LEDMatrixDriver::display();
}
void RowColLEDMatrixDriver::clear() {
  LEDMatrixDriver::clear();
}

void RowColLEDMatrixDriver::setPixel(uint16_t x, uint16_t y, bool enabled) {
  while (y >= PIXELS) {
    x = x + (PIXELS * W);
    y = y - PIXELS;
  }
  LEDMatrixDriver::setPixel(x,y, enabled);
}

bool RowColLEDMatrixDriver::getPixel(uint16_t x, uint16_t y) const {
  while (y >= 8) {
    x = x + (8 * W);
    y = y - 8;
  }
  return LEDMatrixDriver::getPixel(x, y);
}

void RowColLEDMatrixDriver::fadeDown(int fadeDelay) {
  for (int level = intensity; level >= 0; level--) {
    LEDMatrixDriver::setIntensity(level);
    Serial.print("fadedown to ");Serial.println(level);
    delay(fadeDelay);
  }
}

void RowColLEDMatrixDriver::fadeUp(int fadeDelay) {
  for (int level = 0; level <= intensity; level++) {
    LEDMatrixDriver::setIntensity(level);
    delay(fadeDelay);
  }
}


#endif
