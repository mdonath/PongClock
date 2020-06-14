#ifndef _Font_HPP_
#define _Font_HPP_

#include <Arduino.h>

class Font {
  public:
    Font();

    virtual byte* getChar(char c) = 0;
    virtual uint8_t getWidth() = 0;
    virtual uint8_t getHeight() = 0;

  private:
  
};

class FixedFont : public Font {
  public:
    FixedFont(uint8_t charWidth, uint8_t charHeight);
    uint8_t getWidth();
    uint8_t getHeight();

  protected:
    const uint8_t width;
    const uint8_t height;
};

#endif
