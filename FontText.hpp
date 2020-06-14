#ifndef _FontText_HPP_
#define _FontText_HPP_

#include "RowColLEDMatrixDriver.hpp"
#include "Font.hpp"
#include "Text.hpp"

class FontText : public Text {
  public:
    FontText(int x, int y, String& text, Font *font, RowColLEDMatrixDriver *lmd);

    void draw();
    void drawSprite(byte* sprite, int x, int y, int width, int height);

    static int drawCharacter(int x, int y, char c, Font *font, RowColLEDMatrixDriver *lmd) {
      byte* glyph = font->getChar(c);
      FontText::drawGlyph(glyph, x, y, font->getWidth(), font->getHeight(), lmd);
      return x + font->getWidth();
    }
  private:
    Font* font;
    static void drawGlyph(byte* sprite, int x, int y, int width, int height, RowColLEDMatrixDriver *lmd) {
      // The mask is used to get the column bit from the sprite row
      byte mask = B10000000;

      int maxWidth = lmd->getMatrixWidth();
      int maxHeight = lmd->getMatrixHeight();

      for ( int iy = 0; iy < height; iy++ ) {
        for ( int ix = 0; ix < width; ix++ ) {
          if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
            byte b;
            if (ix < 8) {
              b = sprite[iy];
              mask = 128 >> ix;
            } else {
              b = sprite[iy + height];
              mask = 128 >> (ix % 8);
            }
            lmd->setPixel(
              x + ix,
              y + iy,
              (bool)(b & mask )
            );
          }
        }
      }
    }
};

FontText::FontText(int x, int y, String& text, Font *font, RowColLEDMatrixDriver *lmd):
  Text(x, y, text, lmd),
  font(font)
{
  // empty
}

void FontText::draw() {
  for (int idx = 0; idx < len; idx++)  {

    // stop if char is outside visible area
    if ( x + idx * font->getWidth() > lmd->getMatrixWidth() ) {
      return;
    }

    // only draw if char is visible
    if (lmd->getPixelSize() + x + idx * font->getWidth() > 0) {
      char c = text[idx];
      byte* glyph = font->getChar(c);
      drawSprite(glyph, x + idx * font->getWidth(), y, font->getWidth(), font->getHeight() );
    }
  }
}

void FontText::drawSprite(byte* sprite, int x, int y, int width, int height) {
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  int maxWidth = lmd->getMatrixWidth();
  int maxHeight = lmd->getMatrixHeight();

  for ( int iy = 0; iy < height; iy++ ) {
    for ( int ix = 0; ix < width; ix++ ) {
      if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
        byte b;
        if (ix < 8) {
          b = sprite[iy];
          mask = 128 >> ix;
        } else {
          b = sprite[iy + height];
          mask = 128 >> (ix % 8);
        }
        lmd->setPixel(
          x + ix,
          y + iy,
          (bool)(b & mask )
        );
      }
    }
  }
}
#endif
