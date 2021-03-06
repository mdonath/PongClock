#ifndef _MyFont_HPP_
#define _MyFont_HPP_

// copied from http://heim.ifi.uio.no/haakoh/avr/a
const int myfont_count = 80;

byte PROGMEM myfont[80][5] = {
  {0, 0, 0, 0, 0}, // space
  {0x3f, 0x48, 0x48, 0x48, 0x3f}, // A
  {0x7f, 0x49, 0x49, 0x49, 0x36},
  {0x3e, 0x41, 0x41, 0x41, 0x22},
  {0x7f, 0x41, 0x41, 0x22, 0x1c},
  {0x7f, 0x49, 0x49, 0x49, 0x41},
  {0x7f, 0x48, 0x48, 0x48, 0x40},
  {0x3e, 0x41, 0x49, 0x49, 0x2e},
  {0x7f, 0x08, 0x08, 0x08, 0x7f},
  {0x00, 0x41, 0x7f, 0x41, 0x00},
  {0x06, 0x01, 0x01, 0x01, 0x7e},
  {0x7f, 0x08, 0x14, 0x22, 0x41},
  {0x7f, 0x01, 0x01, 0x01, 0x01},
  {0x7f, 0x20, 0x10, 0x20, 0x7f},
  {0x7f, 0x10, 0x08, 0x04, 0x7f},
  {0x3e, 0x41, 0x41, 0x41, 0x3e},
  {0x7f, 0x48, 0x48, 0x48, 0x30},
  {0x3e, 0x41, 0x45, 0x42, 0x3d},
  {0x7f, 0x48, 0x4c, 0x4a, 0x31},
  {0x31, 0x49, 0x49, 0x49, 0x46},
  {0x40, 0x40, 0x7f, 0x40, 0x40},
  {0x7e, 0x01, 0x01, 0x01, 0x7e},
  {0x7c, 0x02, 0x01, 0x02, 0x7c},
  {0x7f, 0x02, 0x04, 0x02, 0x7f},
  {0x63, 0x14, 0x08, 0x14, 0x63},
  {0x60, 0x10, 0x0f, 0x10, 0x60},
  {0x43, 0x45, 0x49, 0x51, 0x61},  // Z

  {0x01, 0x00, 0x00, 0x00, 0x00},  // .
  {0x00, 0x00, 0x70, 0x00, 0x00},  // '
  {0x00, 0x00, 0x14, 0x00, 0x00},  // :
  {0x7F, 0x22, 0x14, 0x08, 0x00},  // selector arrow
  {0x00, 0x08, 0x08, 0x08, 0x00},  // -

  {0x3E, 0x45, 0x49, 0x51, 0x3E}, // 0 - 9
  {0x00, 0x21, 0x7F, 0x01, 0x00},
  {0x21, 0x43, 0x45, 0x49, 0x31},
  {0x22, 0x41, 0x49, 0x49, 0x36},
  {0x0C, 0x14, 0x24, 0x7F, 0x04},
  {0x72, 0x51, 0x51, 0x51, 0x4E},
  {0x3E, 0x49, 0x49, 0x49, 0x26},
  {0x40, 0x40, 0x4F, 0x50, 0x60},
  {0x36, 0x49, 0x49, 0x49, 0x36},
  {0x32, 0x49, 0x49, 0x49, 0x3E},

  {0x2,  0x15, 0x15, 0x15, 0xF },  // a
  {0x7F, 0x5,  0x9,  0x9,  0x6 },
  {0xE,  0x11, 0x11, 0x11, 0x2 },
  {0x6,  0x9,  0x9,  0x5,  0x7F},
  {0xE,  0x15, 0x15, 0x15, 0xC },
  {0x8,  0x3F, 0x44, 0x40, 0x20},  // f
  {0x18, 0x25, 0x25, 0x25, 0x3E},
  {0x7F, 0x8,  0x10, 0x10, 0xF},
  {0x0,  0x0,  0x2F, 0x0,  0x0},
  {0x2,  0x1,  0x11, 0x5E, 0x0},
  {0x7F, 0x4,  0x6,  0x11, 0x0},
  {0x0,  0x41, 0x7F, 0x1,  0x0},  // l
  {0x1F, 0x10, 0xC,  0x10, 0xF},
  {0x1F, 0x8,  0x10, 0x10, 0xF},
  {0xE,  0x11, 0x11, 0x11, 0xE},
  {0x1F, 0x14, 0x14, 0x14, 0x8},
  {0x8,  0x14, 0x14, 0xC,  0x1F}, // q
  {0x1F, 0x8,  0x10, 0x10, 0x8},
  {0x9,  0x15, 0x15, 0x15, 0x2},
  {0x10, 0x7E, 0x11, 0x1,  0x2},
  {0x1E, 0x1,  0x1,  0x2,  0x1F}, // u
  {0x1C, 0x2,  0x1,  0x2,  0x1C},
  {0x1E, 0x1,  0x3,  0x1,  0x1E},
  {0x11, 0xA,  0x4,  0xA,  0x11},
  {0x18, 0x5,  0x5,  0x5,  0x1E},
  {0x11, 0x13, 0x15, 0x19, 0x11}, // z

  // invader sprites - may still need some tweaking
  {0x0,  0x19, 0x3A, 0x6D, 0x7A}, // top invader left
  {0x7A, 0x6D, 0x3A, 0x19, 0x0} , // top invader right
  {0x0,  0x1A, 0x3D, 0x68, 0x7C}, // top invader left wiggle
  {0x7C, 0x68, 0x3D, 0x1A, 0x0},  // top invader right wiggle
  {0x38, 0xD,  0x5E, 0x36, 0x1C}, // middle invader left
  {0x1C, 0x36, 0x5E, 0xD,  0x38},
  {0xE,  0xC,  0x5E, 0x35, 0x1C}, // middle invader left wiggle
  {0x1C, 0x35, 0x5E, 0xC,  0xE},
  {0x19, 0x39, 0x3A, 0x6C, 0x7A}, // bottom invader left
  {0x7A, 0x6C, 0x3A, 0x39, 0x19},
  {0x18, 0x39, 0x3B, 0x6C, 0x7C}, // bottom invader left wiggle
  {0x7C, 0x6C, 0x3B, 0x39, 0x18},
};

class MyFont : public FixedFont {
  public:
    MyFont();
    byte* getChar(char c);

  private:
    char getIndex(char c);
};

MyFont::MyFont():
  FixedFont(5, 7) {
}

byte* MyFont::getChar(char c) {
  int index = getIndex(c);
  if (index < 0 || index >= myfont_count) {
    return NULL;
  }
  return myfont[index]; 
}

char MyFont::getIndex(char c) {
  if (c >= 'A' && c <= 'Z' ) {
    c &= 0x1F;   // A-Z maps to 1-26
  }
  else if (c >= 'a' && c <= 'z') {
    c = (c - 'a') + 42;   // a-z maps to 42-68
  }
  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 32;
  }
  else if (c == ' ') {
    c = 0; // space
  }
  else if (c == '.') {
    c = 27; // full stop
  }
  else if (c == '\'') {
    c = 28; // single quote mark
  }
  else if (c == ':') {
    c = 29; // colon
  }
  else if (c == '>') {
    c = 30; // clock_mode selector arrow
  }
  else if (c == '-') {
    c = 31; // minus
  }
  return c;
}
#endif
