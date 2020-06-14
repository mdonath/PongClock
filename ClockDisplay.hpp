#ifndef _ClockDisplay_HPP_
#define _ClockDisplay_HPP_

#include <Arduino.h>

#include "RowColLEDMatrixDriver.hpp"
#include "UserInterfaceInput.hpp"
#include "ClockMode.hpp"
#include "Clock.hpp"
#include "Font.hpp"


#define NEXT_DATE_MIN 10                 // After the date has been displayed automatically, the next time it's displayed is at least 10 mintues later
#define NEXT_DATE_MAX 15                 // After the date has been displayed automatically, the next time it's displayed is at most 15 mintues later

#define NUM_DISPLAY_MODES 11              // Number display modes (counting zero as the first mode)
#define NUM_SETTINGS_MODES 5             // Number settings modes = 6 (counting zero as the first mode)

#define X_MAX 47                         // Matrix X max LED coordinate (for 2 displays placed next to each other)
#define Y_MAX 15                         // Matrix Y max LED coordinate (for 2 displays placed next to each other)

#define FADEDELAY 300                     // Time to fade display to black


/**

*/
class ClockDisplay {
  public:
    ClockDisplay(UserInterfaceInput* uiInput);
    void setup();
    void printVersion();
    void loop();

    bool handleButtons();

    void cls();
    void fadeDown();
    void setIntensity(byte brightness);
    void fadeUp();
    void plot(char x, char y, char v);
    void setPixel(char x, char y, char v);
    bool getPixel(char x, char y);
    void display();

    void plotLevelbar(byte xpos, byte ypos, byte xbar, byte ybar);
    void drawBox(int x1, int y1, int x2, int y2);
    void scroll(String message);

    void putTinyChar(byte x, byte y, char c);
    void putNormalChar(byte x, byte y, char c);
    void putBigChar(byte x, byte y, char c);

    void putTinyString(byte x, byte y, String& s);
    void putTinyString(byte x, byte y, String& s, long delayTime);
    void putNormalString(byte x, byte y, String& s);
    void putBigString(byte x, byte y, String& s);
    void putFontString(byte x, byte y, String& s, long delayTime, Font* font);

    bool keepModeRunning();
    void switchModeMenu();
    void restoreMode();
    void changeToNextMode();
    bool changeToMode(String& message);

    void setNextRandom();

    void displayDate();
    void set_next_date();
    bool check_show_date();
    void flashing_cursor(byte xpos, byte ypos, byte cursor_width, byte cursor_height, byte repeats);

    UserInterfaceInput* getUserInterfaceInput();
    Clock *getClock();
    bool getConfigurationAmPm();
    int getConfigurationVolume();
    bool getConfigurationRandomModeEnabled();

    RowColLEDMatrixDriver* lmd;

  private:
    
    UserInterfaceInput* uiInput;

    ClockMode* clockModes[NUM_DISPLAY_MODES];
    bool mustSwitch = false;
    byte changeModeTime = 0;        // Holds hour when clock mode will next change if in random mode.
    ClockMode* currentMode;           // Default clock mode. Default = 0 (normal)
    ClockMode* oldMode;              // Stores the previous clock mode, so if we go to date or whatever, we know what mode to go back to after.

    Clock* clock;
    byte next_display_date;           // Holds the minute at which the date is automatically next displayed
};

#endif
