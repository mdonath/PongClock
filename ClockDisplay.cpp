#ifndef _ClockDisplay_CPP_
#define _ClockDisplay_CPP_

#include <WiFi.h>

#include "ClockDisplay.hpp"

#include "TinyFont.hpp"
#include "BigFont.hpp"
#include "MyFont.hpp"
#include "PongFontText.hpp"
#include "FontText.hpp"

#include "NormalClockMode.hpp"
#include "DigitsClockMode.hpp"
#include "SlideClockMode.hpp"
#include "PongClockMode.hpp"
#include "JumbleClockMode.hpp"
#include "WordClockMode.hpp"
#include "SetupClockMode.hpp"
#include "DateClockMode.hpp"
#include "DutchWordClockMode.hpp"
#include "Breakout.hpp"
#include "ScrollerClockMode.hpp"


// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are arduino specific SPI pins (MOSI=DIN of the LEDMatrix and CLK) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 17;

const uint8_t LEDMATRIX_PIXELS = 8;
const uint8_t LEDMATRIX_SEGMENTS_WIDTH = 6;
const uint8_t LEDMATRIX_SEGMENTS_HEIGHT = 2;
const uint8_t LEDMATRIX_WIDTH = (LEDMATRIX_SEGMENTS_WIDTH * LEDMATRIX_PIXELS);
const uint8_t LEDMATRIX_HEIGHT = (LEDMATRIX_SEGMENTS_HEIGHT * LEDMATRIX_PIXELS);

const byte START_INDEX = 4;

ClockDisplay::ClockDisplay(UserInterfaceInput* uiInput):
  uiInput(uiInput)
{
  lmd = new RowColLEDMatrixDriver(LEDMATRIX_SEGMENTS_WIDTH, LEDMATRIX_SEGMENTS_HEIGHT, LEDMATRIX_CS_PIN, LEDMATRIX_PIXELS);
  clock = new Clock();
  clockModes[0] = new SetupClockMode("Setup", this);
  clockModes[1] = new DateClockMode("Date", this);
  clockModes[2] = new NormalClockMode("Normal", this);
  clockModes[3] = new DigitsClockMode("Digits", this);
  clockModes[4] = new SlideClockMode("Slide", this);
  clockModes[5] = new PongClockMode("Pong", this);
  clockModes[6] = new JumbleClockMode("Jumble", this);
  clockModes[7] = new WordClockMode("Words", this);
  clockModes[8] = new DutchWordClockMode("Dutch", this);
  clockModes[9] = new BreakoutClockMode("Break", this);
  clockModes[10] = new ScrollerClockMode("Scroll", this);

  //((ScrollerClockMode *)clockModes[10])->setText("HELLO");

  for (int i = 0; i < NUM_DISPLAY_MODES; i++) {
    clockModes[i]->setNext(clockModes[(i + 1) % NUM_DISPLAY_MODES]);
  }

  currentMode = clockModes[START_INDEX];
  oldMode = clockModes[START_INDEX];
}

void ClockDisplay::scroll(String message) {
  Serial.print(">");Serial.print(message);Serial.println("<");
  cls();
  ((ScrollerClockMode *)clockModes[10])->setText(message);
  String newMode("Scroll");
  changeToMode(newMode);
}

bool ClockDisplay::changeToMode(String& modeName) {
  for (int i = 0; i < NUM_DISPLAY_MODES; i++) {
    if (modeName == (clockModes[i]->getName(), modeName)) {
      oldMode = currentMode;
      currentMode = clockModes[i];
      mustSwitch = true;
      return true;
    }
  }
  return false;
}


bool ClockDisplay::getConfigurationAmPm() {
  return ((SetupClockMode*)clockModes[0])->getAmPm();
}
int ClockDisplay::getConfigurationVolume() {
  return ((SetupClockMode*)clockModes[0])->getVolume();
}
bool ClockDisplay::getConfigurationRandomModeEnabled() {
  return ((SetupClockMode*)clockModes[0])->getRandomModeEnabled();
}

UserInterfaceInput* ClockDisplay::getUserInterfaceInput() {
  return uiInput;
}

void ClockDisplay::setup() {
  Serial.print("ClockDisplay::setup... ");
  lmd->setEnabled(false);
  lmd->setEnabled(true);
  lmd->setIntensity(0);   // 0 = low, 10 = high
  randomSeed(analogRead(1));
  Serial.println("OK");
}

void ClockDisplay::loop() {
  //run the clock with whatever mode is set by currentMode - the default is set at top of code.
  currentMode->action();
}

bool ClockDisplay::handleButtons() {
  //check buttons
  if (uiInput->isButtonAPressed()) {
    switchModeMenu();
    return true;
  }
  if (uiInput->isButtonBPressed()) {
    displayDate();
    fadeDown();
    return true;
  }
  return false;
}

void ClockDisplay::restoreMode() {
  currentMode = oldMode;
}

void ClockDisplay::changeToNextMode() {
  while ((currentMode = currentMode->getNext()) == clockModes[0]) {
    Serial.println(currentMode->getName());
  }
  mustSwitch = true;
}

//display menu to change the clock mode
void ClockDisplay::switchModeMenu() {
  Serial.println("Entering switchModeMenu()");

  //remember mode we are in. We use this value if we go into settings mode, so we can change back from settings mode (6) to whatever mode we were in.
  oldMode = currentMode;

  byte firstrun = 1;

  //loop waiting for button (timeout after 35 loops to return to mode X)
  for (int count = 0; count < 35 ; count++) {

    //if user hits button, change the currentMode
    if (uiInput->isButtonAPressed() || firstrun == 1) {

      count = 0;
      cls();

      if (firstrun == 0) {
        currentMode = currentMode->getNext();
      }

      //print arrow and current clock_mode name on line one and print next clock_mode name on line two
      String str_top(">");
      str_top += currentMode->getName();

      String str_bot(" ");
      str_bot += currentMode->getNext()->getName();

      putNormalString(0, 0, str_top);
      putNormalString(0, 8, str_bot);

      firstrun = 0;
    }
    delay(50);
  }
  Serial.println("Leaving switchModeMenu()");
}

void ClockDisplay::setIntensity(byte brightness) {
  lmd->setIntensity(brightness);
}


//set the next minute the date should be automatically displayed
void ClockDisplay::set_next_date() {
  clock->updateTime();

  next_display_date = clock->getMinutes() + random(NEXT_DATE_MIN, NEXT_DATE_MAX);
  //check we've not gone over 59
  if (next_display_date >= 59) {
    next_display_date = random(NEXT_DATE_MIN, NEXT_DATE_MAX);
  }
}

void ClockDisplay::cls() {
  lmd->clear();
  lmd->display();
}

void ClockDisplay::fadeDown() {
  lmd->fadeDown(FADEDELAY);
}

void ClockDisplay::fadeUp() {
  lmd->fadeUp(FADEDELAY);
}

void ClockDisplay::plot(char x, char y, char v) {
  lmd->setPixel(x, y, v);
  lmd->display();
}

void ClockDisplay::setPixel(char x, char y, char v) {
  lmd->setPixel(x, y, v);
}
void ClockDisplay::display() {
  lmd->display();
}
bool ClockDisplay::getPixel(char x, char y) {
  return lmd->getPixel(x, y);
}

//flashing_cursor
//print a flashing_cursor at xpos, ypos and flash it repeats times
void ClockDisplay::flashing_cursor(byte xpos, byte ypos, byte cursor_width, byte cursor_height, byte repeats) {
  for (byte r = 0; r <= repeats; r++) {
    for (byte x = 0; x <= cursor_width; x++) {
      for (byte y = 0; y <= cursor_height; y++) {
        plot(x + xpos, y + ypos, 1);
      }
    }

    if (repeats > 0) {
      delay(400);
    }
    else {
      delay(70);
    }

    for (byte x = 0; x <= cursor_width; x++) {
      for (byte y = 0; y <= cursor_height; y++) {
        plot(x + xpos, y + ypos, 0);
      }
    }
    //if cursor set to repeat, wait a while
    if (repeats > 0) {
      delay(400);
    }
  }
}

void ClockDisplay::putTinyChar(byte x, byte y, char c) {
  TinyFont font;
  PongFontText::drawCharacter(x, y, c, &font, lmd);
  lmd->display();
}
void ClockDisplay::putNormalChar(byte x, byte y, char c) {
  MyFont font;
  PongFontText::drawCharacter(x, y, c, &font, lmd);
  lmd->display();
}
void ClockDisplay::putBigChar(byte x, byte y, char c) {
  BigFont font;
  FontText::drawCharacter(x, y, c, &font, lmd);
  //lmd->display();
}

void ClockDisplay::putTinyString(byte x, byte y, String& s) {
  putTinyString(x, y, s, 0);
}

void ClockDisplay::putTinyString(byte x, byte y, String& s, long delayTime) {
  TinyFont font;
  putFontString(x, y, s, delayTime, &font);
  lmd->display();
}
void ClockDisplay::putNormalString(byte x, byte y, String& s) {
  MyFont font;
  putFontString(x, y, s, 0, &font);
  lmd->display();
}

void ClockDisplay::putBigString(byte x, byte y, String& s) {
  BigFont font;
  putFontString(x, y, s, 0, &font);
//  lmd->display();
}

void ClockDisplay::putFontString(byte x, byte y, String& s, long delayTime, Font* font) {
  for (int i = 0; i < s.length(); i++) {
    x = 1 + PongFontText::drawCharacter(x, y, s[i], font, lmd);
    if (delayTime > 0) {
      lmd->display();
      delay(delayTime);
    }
  }
}


//run clock main loop as long as keepModeRunning returns true
bool ClockDisplay::keepModeRunning() {

  if (mustSwitch) {
    mustSwitch = false;
    fadeDown();
    return false;
  }

  // if random mode is on... check the hour when we change mode.
  if (getConfigurationRandomModeEnabled()) {
    // if hour value in change mode time = hours. then reurn false = i.e. exit mode.
    if (changeModeTime == clock->getHours()) {
      // set the next random clock mode and time to change it
      setNextRandom();
      // exit the current mode.
      fadeDown();
      return false;
    }
  }

  clock->updateTime();

  if (handleButtons()) {
    return false;
  }

  // keep running in this mode
  return true;
}

//set the next hour the clock will change mode when random mode is on
void ClockDisplay::setNextRandom() {

  //set the next hour the clock mode will change - current time plus 2 - 4 hours
  clock->updateTime();
  changeModeTime = clock->getHours() + random (2, 5);

  //if changeModeTime now happens to be over 23, then set it to between 1 and 3am
  if (changeModeTime > 23) {
    changeModeTime = random (1, 4);
  }

  //set the new clock mode
  currentMode = currentMode->getNext(); // = random(0, NUM_DISPLAY_MODES + 1);  //pick new random clock mode
}

void ClockDisplay::drawBox(int x1, int y1, int x2, int y2) {
  // draw box
  for (byte x = x1; x <= x2; x++) {
    setPixel(x, y1, 1);
    setPixel(x, y2, 1);
  }
  for (byte y = y1; y <= y2; y++) {
    setPixel(x1, y, 1);
    setPixel(x2, y, 1);
  }
  display();
}

//power up led test & display software version number
void ClockDisplay::printVersion() {
  // test all leds.
  plotLevelbar(0, 0, 48, 16);

  delay(1000);
  fadeDown();
  cls();

  //draw box
  drawBox(0, 0 , 47, 15);

  // top line
  String ver_top("Pong Clock");
  putTinyString(2, 1, ver_top, 35);

  //bottom line
  String ver_bot("Version 7.0");
  putTinyString(2, 8, ver_bot, 35);

  delay(4000);
  fadeDown();
}

// display a horizontal bar on the screen at offset xpos by ypos with height and width of xbar, ybar
void ClockDisplay::plotLevelbar(byte xpos, byte ypos, byte width, byte height) {
  for (byte x = 0; x <= width; x++) {
    for (byte y = 0; y <= height; y++) {
      plot(x + xpos, y + ypos, 1);
    }
  }
}

//displayDate - print the day of week, date and month with a flashing cursor effect
void ClockDisplay::displayDate() {
  clockModes[1]->action();
}

//if the minute matches next_display_date then display the date, and set the next minute the date should be displayed
bool ClockDisplay::check_show_date() {
  if (clock->getMinutes() == next_display_date) {
    fadeDown();
    displayDate();
    fadeDown();
    set_next_date();
    return true; // the date was shown so run whatever setup you need (e.g redraw the pong pitch)
  }
  return false; //the date wasn't shown.
}

Clock* ClockDisplay::getClock() {
  return clock;
}

#endif
