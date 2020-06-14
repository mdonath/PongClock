#ifndef _BigText_HPP_
#define _BigText_HPP_

#include "RowColLEDMatrixDriver.hpp"
#include "font.h"

class BigText : public Text {
  public:
    BigText(int x, int y, char* text, RowColLEDMatrixDriver *lmd);

    void draw();
    void drawSprite(byte* sprite, int x, int y, int width, int height);
  private:

};

BigText::BigText(int x, int y, char* text, RowColLEDMatrixDriver *lmd):
  Text(x, y, text, lmd) {
  // empty
}

void BigText::draw() {
  for ( int idx = 0; idx < len; idx ++ )  {

    // stop if char is outside visible area
    if ( x + idx * lmd->getPixelSize()  > lmd->getMatrixWidth() ) {
      return;
    }

    // only draw if char is visible
    if (lmd->getPixelSize() + x + idx * lmd->getPixelSize() > 0) {
      int c = text[idx] - 32;
      drawSprite(font[c], x + idx * lmd->getPixelSize(), y, 8, 8 );
    }
  }
}
void BigText::drawSprite(byte* sprite, int x, int y, int width, int height) {
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  int maxWidth = lmd->getMatrixWidth();
  int maxHeight = lmd->getMatrixHeight();

  for ( int iy = 0; iy < height; iy++ ) {
    for ( int ix = 0; ix < width; ix++ ) {
      if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
        lmd->setPixel(
          x*2 + ix * 2,
          y*2 + iy * 2,
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
