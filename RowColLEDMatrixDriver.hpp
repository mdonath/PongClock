#ifndef _RowColLEDMatrixDriver_HPP_
#define _RowColLEDMatrixDriver_HPP_

#include <Arduino.h>
#include <LEDMatrixDriver.hpp>

/**
 * 
 */
class RowColLEDMatrixDriver : private LEDMatrixDriver {
  public:
    RowColLEDMatrixDriver(uint8_t W, uint8_t H, uint8_t ssPin, uint8_t PIXELS);

    void setEnabled(bool enabled);
    void setIntensity(uint8_t level);

    void setPixel(uint16_t x, uint16_t y, bool enabled);
    bool getPixel(uint16_t x, uint16_t y) const;
    int getPixelSize();
    int getMatrixWidth();
    int getMatrixHeight();

    void clear();
    void display();
    void fadeDown(int delay);
    void fadeUp(int delay);

  private:
    const uint8_t W;
    const uint8_t H;
    const uint8_t PIXELS;

    byte intensity = 15;

};

#endif

