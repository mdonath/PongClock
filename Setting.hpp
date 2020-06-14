#ifndef _Setting_HPP_
#define _Setting_HPP_

#include <Arduino.h>
#include "ClockMode.hpp"

class Setting {
  public:
    Setting(const String& name);

    virtual void setup(ClockDisplay* clockDisplay) = 0;

    void setNext(Setting* next);
    void setName(const String& name);
    String getName();
    Setting* getNext();

  protected:
    String name;

  private:
    Setting* next;

    ClockDisplay* clockDisplay;
};

Setting::Setting(const String& name) : name(name) {
  // NOP
}

void Setting::setName(const String& newName) {
  name = newName;
}

String Setting::getName() {
  return name;
}
Setting* Setting::getNext() {
  return next;
}

void Setting::setNext(Setting* nextSetting) {
  next = nextSetting;
}


////////////////////


class BrightnessSetting : public Setting {
  public:
    BrightnessSetting();
    void setup(ClockDisplay* clockDisplay);

    byte getLevel();
  private:
    byte level = 15; // Screen brightness - default is 15 which is brightest.
};

BrightnessSetting::BrightnessSetting() : Setting(String("Bright")) {
  // NOP
}

void BrightnessSetting::setup(ClockDisplay* clockDisplay) {
  clockDisplay->cls();

  // print "brightness"
  String text("Bright");
  clockDisplay->putNormalString(6, 0, text);

  // wait for button input
  while (!clockDisplay->getUserInterfaceInput()->isButtonAPressed()) {

    clockDisplay->plotLevelbar(1, 8, level * 3, 6); //display the brightness level as a bar
    while (clockDisplay->getUserInterfaceInput()->isButtonBPressed()) {
      if (level == 15) {
        level = 0;
        clockDisplay->cls();
      } else {
        level++;
      }
      //print the new value
      clockDisplay->putNormalString(6, 0, text);
      clockDisplay->plotLevelbar(1, 8, level * 3, 6); //display the brightness level as a bar
      clockDisplay->setIntensity(level);
      delay(150);
    }
  }
}

////////////////////

class VolumeSetting : public Setting {
  public:
    VolumeSetting();
    void setup(ClockDisplay* clockDisplay);

    byte getLevel();
  private:
    byte level = 10;
};

VolumeSetting::VolumeSetting() : Setting(String("Volume")) {
  // NOP
}

void VolumeSetting::setup(ClockDisplay* clockDisplay) {
  clockDisplay->cls();

  // print "brightness"
  String text(name);
  clockDisplay->putNormalString(6, 0, text);

  PongAudio* pongAudio = new PongAudio();
  pongAudio->setup();
  
  // wait for button input
  while (!clockDisplay->getUserInterfaceInput()->isButtonAPressed()) {

    clockDisplay->plotLevelbar(1, 8, map(level, 0, 255, 0, 48), 6); //display the brightness level as a bar
    while (clockDisplay->getUserInterfaceInput()->isButtonBPressed()) {
      if (level >= 255) {
        level = 0;
        clockDisplay->cls();
      } else {
        level += 5;
        pongAudio->setVolume(level);
        pongAudio->soundPaddle();
      }
      //print the new value
      clockDisplay->putNormalString(6, 0, text);
      clockDisplay->plotLevelbar(1, 8, map(level, 0, 255, 0, 48), 6); //display the volume level as a bar
      delay(150);
    }
  }
}

byte VolumeSetting::getLevel() {
  return level;
}

////////////////////

/**
   Toggle random mode - pick a different clock mode every few hours
*/
class RandomModeSetting : public Setting {
  public:
    RandomModeSetting();
    void setup(ClockDisplay* clockDisplay);

    bool getEnabled();
  private:
    bool enabled;
};

RandomModeSetting::RandomModeSetting() : Setting(String("Random")) {
  // NOP
}

void RandomModeSetting::setup(ClockDisplay* clockDisplay) {
  clockDisplay->cls();

  String optionOn("Mode On ");
  String optionOff("Mode Off");

  //if random mode is on, turn it off
  if (enabled) {
    enabled = false;

    //print a message on the display
    clockDisplay->putNormalString(0, 0, name);
    clockDisplay->putNormalString(0, 8, optionOff);

  } else {
    enabled = true;

    //set hour mode will change
    clockDisplay->setNextRandom();

    //print a message on the display
    clockDisplay->putNormalString(0, 0, name);
    clockDisplay->putNormalString(0, 8, optionOn);
  }
  delay(1500); // leave the message up for a second or so
}

bool RandomModeSetting::getEnabled() {
  return enabled;
}

////////////////////

/**
   Set 12 or 24 hour clock
*/
class AmPmSetting : public Setting {
  public:
    AmPmSetting();
    void setup(ClockDisplay* clockDisplay);

    bool getAmPm();
  private:
    bool ampm = false; // false => 24 hours, true => 12 hours
    String* option12;
    String* option24;
};

AmPmSetting::AmPmSetting() : Setting(String("24 Hour")) {
  option12 = new String("12 Hour");
  option24 = new String("24 Hour");
}

/**
   Define 12 or 24 hour time. false = 24 hour. true = 12 hour
*/
void AmPmSetting::setup(ClockDisplay* clockDisplay) {
  clockDisplay->cls();

  // AM/PM or 24 hour clock mode - flip the bit (makes 0 into 1, or 1 into 0 for ampm mode)
  ampm = (ampm ^ 1);

  //print confirmation message
  if (ampm) {
    clockDisplay->putNormalString(3, 4, *option12);
    name = *option24;
  } else {
    clockDisplay->putNormalString(3, 4, *option24);
    name = *option12;
  }
  delay(1500);
}

bool AmPmSetting::getAmPm() {
  return ampm;
}


//////////////////




class DaylightSetting : public Setting {
  public:
    DaylightSetting();
    void setup(ClockDisplay* clockDisplay);

    bool getDaylight();
  private:
    bool daylightMode = false;
};

DaylightSetting::DaylightSetting() : Setting(String("DST Adj")) {
  // NOP
}

//add or remove an hour for daylight savings
void DaylightSetting::setup(ClockDisplay* clockDisplay) {
  clockDisplay->cls();

  String title("Daylight");
  String optionOn("Mode On ");
  String optionOff("Mode Off");

  clockDisplay->getClock()->updateTime();
  byte hr = clockDisplay->getClock()->getHours();

  //if daylight mode if on, turn it off
  if (daylightMode) {

    //turn random mode off
    daylightMode = false;

    //take one off the hour
    if (hr > 0) {
      hr--;
    } else {
      hr = 23;
    }

    //print a message on the display
    clockDisplay->putNormalString(0, 0, title);
    clockDisplay->putNormalString(0, 8, optionOff);
  } else {
    //turn daylight mode on.
    daylightMode = true;

    //add one to the hour
    if (hr < 23) {
      hr++;
    } else {
      hr = 0;
    }

    //print a message on the display
    clockDisplay->putNormalString(0, 0, title);
    clockDisplay->putNormalString(0, 8, optionOn);
  }

  //write the change to the clock chip
  //DateTime now = ds1307.now();
  //ds1307.adjust(DateTime(now.year(), now.month(), now.day(), hr, now.minute(), now.second()));

  delay(1500); //leave the message up for a second or so
}

bool DaylightSetting::getDaylight() {
  return daylightMode;
}

///////////////////


class GoBackSetting : public Setting {
  public:
    GoBackSetting();
    void setup(ClockDisplay* clockDisplay);

  private:
};

GoBackSetting::GoBackSetting() : Setting(String("Go Back")) {
  // NOP
}

void GoBackSetting::setup(ClockDisplay* clockDisplay) {
  // NOP
}

///////////////////

#endif
