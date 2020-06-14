#ifndef _AbstractWordClockMode_CPP_
#define _AbstractWordClockMode_CPP_

#include <Arduino.h>
#include "AbstractClockMode.hpp"


class AbstractWordClockMode : public AbstractClockMode {
  public:
    AbstractWordClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();

  protected:
    virtual void fillLines(byte hours, byte minutes, char *str_top, char *str_bot) = 0;
};


AbstractWordClockMode::AbstractWordClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}

void AbstractWordClockMode::action() {
  ENTER(name);

  byte mins = 100;

  // run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    byte newMins = clockDisplay->getClock()->getMinutes();
    if (mins != newMins) {
      mins = newMins;

      byte hours = clockDisplay->getClock()->getHours();

      char str_top[12];
      char str_bot[12];
      fillLines(hours, mins, str_top, str_bot);
      clockDisplay->fadeDown();
      clockDisplay->cls();
      centerAndPlot(str_top, 1);
      centerAndPlot(str_bot, 8);
    }
    delay(50);
  }
  clockDisplay->fadeDown();
  LEAVE(name);
}

#endif
