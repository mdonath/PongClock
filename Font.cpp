#ifndef _Font_CPP_
#define _Font_CPP_

#include "Font.hpp"

Font::Font() {
  // NOP
}

FixedFont::FixedFont(uint8_t charWidth, uint8_t charHeight):
  Font(),
  width(charWidth),
  height(charHeight) {
  // empty
}

uint8_t FixedFont::getWidth() {
  return width;
}

uint8_t FixedFont::getHeight() {
  return height;
}

#endif
