#ifndef _BigFont_HPP_
#define _BigFont_HPP_

#include "Font.hpp"

const int bigfont_count = 10;
byte PROGMEM mybigfont[10][28] = {
  //                                                                                    |
  {0x3F, 0x7F, 0xE1, 0xC1, 0xC3, 0xC7, 0xCE, 0xDC, 0xF8, 0xF0, 0xE1, 0xE1, 0x7F, 0x3F, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 0 OK
, {0x0C, 0x1C, 0x3C, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 1 OK 
, {0x3F, 0x7F, 0xE1, 0xC0, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xFF, 0xFF, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0} // 2 OK
, {0x3F, 0x7F, 0xE1, 0xC0, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0xC0, 0xE1, 0x7F, 0x3F, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 3 OK
, {0x03, 0x07, 0x0F, 0x1F, 0x3B, 0x73, 0xE3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00} // 4 OK
, {0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0xC0, 0xE1, 0x7F, 0x3F, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 5 OK
, {0x3F, 0x7F, 0xE1, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xC1, 0xC0, 0xC0, 0xC1, 0x7F, 0x3F, 0x00, 0x80, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 6 OK
, {0xFF, 0xFF, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // 7 OK
, {0x3F, 0x7F, 0xE1, 0xC0, 0xC0, 0xE1, 0x7F, 0x7F, 0xE1, 0xC0, 0xC0, 0xC1, 0x7F, 0x3F, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 8 OK
, {0x3F, 0x7F, 0xE0, 0xC0, 0xC0, 0xE0, 0x7F, 0x3F, 0x00, 0x00, 0x00, 0xE1, 0x7F, 0x3F, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00} // 9 OK
  };

class BigFont : public FixedFont {
  public:
    BigFont();
    byte* getChar(char c);

  private:
    const uint8_t width = 10;
    const uint8_t height = 14;
};

BigFont::BigFont():
  FixedFont(10, 14) {
}

byte* BigFont::getChar(char c) {
  int index = c - '0';
  if (index < 0 || index >= bigfont_count) {
    return NULL;
  }
  return mybigfont[index];
}
#endif
