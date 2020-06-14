#ifndef _Text_HPP_
#define _Text_HPP_

#include "RowColLEDMatrixDriver.hpp"
#include "OldFont.hpp"

class Text {
  public:
    Text(int x, int y, String& text, RowColLEDMatrixDriver *lmd);
    void nextStep();
    void draw();
    void drawSprite(byte* sprite, int x, int y, int width, int height);
    void setX(int x);
  protected:
    int x;
    int y;
    String text;
    int len;

    RowColLEDMatrixDriver *lmd;
};

Text::Text(int x, int y, String& text, RowColLEDMatrixDriver *lmd):
  x(x),
  y(y),
  text(text),
  lmd(lmd)
{
  len = text.length();
}

void Text::setX(int newValue) {
  x = newValue;
}

void Text::nextStep() {
  x--;
  if (x < len * -lmd->getPixelSize()) {
    x = lmd->getMatrixWidth();
  }
}

void Text::draw() {
  for (int idx = 0; idx < len; idx++)  {
Serial.print("Drawing ");Serial.println(text[idx]);
    // stop if char is outside visible area
    if ( x + idx * lmd->getPixelSize()  > lmd->getMatrixWidth() ) {
      return;
    }

    // only draw if char is visible
    if (lmd->getPixelSize() + x + idx * lmd->getPixelSize() > 0) {
      int c = text[idx] - 32;
      drawSprite(oldfont[c], x + idx * lmd->getPixelSize(), y, 8, 8 );
    }
  }
}

/**
   This draws a sprite to the given position using the width and height supplied (usually 8x8)
*/
void Text::drawSprite(byte* sprite, int x, int y, int width, int height) {
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  int maxWidth = lmd->getMatrixWidth();
  int maxHeight = lmd->getMatrixHeight();

  for ( int iy = 0; iy < height; iy++ ) {
    for ( int ix = 0; ix < width; ix++ ) {
      if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
        lmd->setPixel(
          x + ix,
          y + iy,
          (bool)(sprite[iy] & mask )
        );
      }

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}

#endif
