#ifndef _PongFontText_HPP_
#define _PongFontText_HPP_

#include "Font.hpp"
#include "Text.hpp"

#include "RowColLEDMatrixDriver.hpp"

class PongFontText : public Text {
  public:
    PongFontText(int x, int y, String& text, Font *font, RowColLEDMatrixDriver *lmd);

    void draw();
    void drawSprite(byte* sprite, int x, int y, int width, int height);

    static int drawCharacter(int x, int y, char c, Font *font, RowColLEDMatrixDriver *lmd) {
      byte* glyph = font->getChar(c);
      PongFontText::drawGlyph(glyph, x, y, font->getWidth(), font->getHeight(), lmd);
      return x + font->getWidth();
    }

  private:
    Font* font;
    static void drawGlyph(byte* sprite, int x, int y, int width, int height, RowColLEDMatrixDriver *lmd) {
      int maxWidth = lmd->getMatrixWidth();
      int maxHeight = lmd->getMatrixHeight();

      for ( int iy = 0; iy < height; iy++ ) {
        for ( int ix = 0; ix < width; ix++ ) {
          if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
            byte b = sprite[ix];
            byte mask = 1 << iy;
            bool value = (bool)(b & mask);
            lmd->setPixel(
              x + ix,
              y + (height - iy),
              value
            );
          }
        }
      }
    }
};

PongFontText::PongFontText(int x, int y, String& text, Font *font, RowColLEDMatrixDriver *lmd):
  Text(x, y, text, lmd),
  font(font)
{
  // empty
}

void PongFontText::draw() {
  for (int idx = 0; idx < len; idx++)  {

    // stop if char is outside visible area
    if ( x + idx * font->getWidth() > lmd->getMatrixWidth() ) {
      return;
    }

    // only draw if char is visible
    if (lmd->getPixelSize() + x + idx * font->getWidth() > 0) {
      char c = text[idx];
      byte* glyph = font->getChar(c);
      drawSprite(glyph, x + (idx * (font->getWidth() + 1)), y, font->getWidth(), font->getHeight() );
    }
  }
}

void PongFontText::drawSprite(byte* sprite, int x, int y, int width, int height) {
  int maxWidth = lmd->getMatrixWidth();
  int maxHeight = lmd->getMatrixHeight();

  for ( int iy = 0; iy < height; iy++ ) {
    for ( int ix = 0; ix < width; ix++ ) {
      if (x + ix >= 0 && y + iy >= 0 && x + ix < maxWidth && y + iy < maxHeight) {
        byte b = sprite[ix];
        byte mask = 1 << iy;
        bool value = (bool)(b & mask);
        lmd->setPixel(
          x + ix,
          y + (height - iy),
          value
        );
      }
    }
  }
}
#endif
