#ifndef _SetupClockMode_HPP_
#define _SetupClockMode_HPP_

#include <Arduino.h>
#include "ClockMode.hpp"
#include "Setting.hpp"


#define NUMBER_OF_SETTINGS 6

/**

*/
class SetupClockMode : public ClockMode {
  public:
    SetupClockMode(const String& name, ClockDisplay* clockDisplay);

    void action();

    bool getDaylightMode();
    bool getAmPm();
    int getVolume();
    byte getBrightness();
    bool getRandomModeEnabled();

  private:
    Setting*            settings[NUMBER_OF_SETTINGS];

    GoBackSetting*      goBack;
    AmPmSetting*        ampm;
    DaylightSetting*    daylight;
    BrightnessSetting*  brightness;
    VolumeSetting*      volume;
    RandomModeSetting*  randomMode;

    ClockDisplay* clockDisplay;
};

SetupClockMode::SetupClockMode(const String& name, ClockDisplay* clockDisplay):
  ClockMode(name),
  clockDisplay(clockDisplay)
{
  settings[0] = goBack = new GoBackSetting();
  settings[1] = randomMode = new RandomModeSetting();
  settings[2] = ampm = new AmPmSetting();
  settings[3] = brightness = new BrightnessSetting();
  settings[4] = daylight = new DaylightSetting();
  settings[5] = volume = new VolumeSetting();

  for (int i = 0; i < NUMBER_OF_SETTINGS; i++) {
    settings[i]->setNext(settings[(i + 1) % NUMBER_OF_SETTINGS]);
  }
}


byte SetupClockMode::getBrightness() {
  return brightness->getLevel();
}
bool SetupClockMode::getDaylightMode() {
  return daylight->getDaylight();
}
bool SetupClockMode::getAmPm() {
  return ampm->getAmPm();
}
int SetupClockMode::getVolume() {
  return volume->getLevel();
}
bool SetupClockMode::getRandomModeEnabled() {
  return randomMode->getEnabled();
}

/**
   display menu to change the clock settings.
*/
void SetupClockMode::action() {
  Setting* currentSetting = goBack;
  bool firstrun = true;

  // loop waiting for button (timeout after 35 loops to return to mode X)
  for (int count = 0; count < 35 ; count++) {

    // if user hits button, change the clock_mode
    if (clockDisplay->getUserInterfaceInput()->isButtonAPressed() || firstrun == 1) {

      count = 0;
      clockDisplay->cls();

      if (!firstrun) {
        currentSetting = currentSetting->getNext();
      }

      // print arrow and current clock_mode name on line one
      String str_top(">");
      str_top += currentSetting->getName();
      clockDisplay->putNormalString(0, 0, str_top);

      // and print next clock_mode name on line two
      String str_bot(" ");
      str_bot += currentSetting->getNext()->getName();
      clockDisplay->putNormalString(0, 8, str_bot);

      firstrun = false;
    }
    delay(50);
  }

  // pick the mode
  currentSetting->setup(clockDisplay);

  // change the clock back to the one it was in before
  clockDisplay->restoreMode();
}


#endif
