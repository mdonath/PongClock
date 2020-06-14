#ifndef _NormalClockMode_HPP_
#define _NormalClockMode_HPP_

#include "AbstractClockMode.hpp"
#include "ClockDisplay.hpp"

/**

*/
class NormalClockMode : public AbstractClockMode {
  public:
    NormalClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();
};

NormalClockMode::NormalClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}

void NormalClockMode::action() {
  ENTER("NormalClockMode");
  bool ampm = clockDisplay->getConfigurationAmPm();

  char textchar[16]; // the 16 characters on the display
  byte mins = 100; //mins
  byte secs = 100; //seconds

  clockDisplay->cls();

  //run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    //if secs changed then update them on the display
    byte newSecs = clockDisplay->getClock()->getSeconds();
    if (secs != newSecs) {
      secs = newSecs;

      //secs
      char buffer[3];
      formatNumber(buffer, secs);
      clockDisplay->putNormalChar(30, 0, ':'); //seconds colon
      clockDisplay->putNormalChar(36, 0, buffer[0]); //seconds
      clockDisplay->putNormalChar(42, 0, buffer[1]); //seconds
    }

    //if minute changes change time
    byte newMins = clockDisplay->getClock()->getMinutes();
    if (mins != newMins) {
      mins = newMins;

      //set characters
      char buffer[3];
      formatHours(buffer, clockDisplay->getClock()->getHours(), ampm);
      textchar[0] = buffer[0];
      textchar[1] = buffer[1];
      textchar[2] = ':';
      formatNumber(buffer, mins);
      textchar[3] = buffer[0];
      textchar[4] = buffer[1];
      textchar[5] = ':';
      formatNumber(buffer, secs);
      textchar[6] = buffer[0];
      textchar[7] = buffer[1];

      //set bottom line
      byte dow  = clockDisplay->getClock()->getDayOfWeek(); // the DS1307 outputs 0 - 6 where 0 = Sunday0 - 6 where 0 = Sunday.
      textchar[8] = getWeekdayShort(dow)[0];
      textchar[9] = getWeekdayShort(dow)[1];
      textchar[10] = ' ';
      formatNumber(buffer, clockDisplay->getClock()->getDay());
      textchar[11] = buffer[0];
      textchar[12] = buffer[1];
      textchar[13] = '-';
      formatNumber(buffer, clockDisplay->getClock()->getMonth());
      textchar[14] = buffer[0];
      textchar[15] = buffer[1];

      //print each char
      for (byte x = 0; x < 8 ; x++) {
        clockDisplay->putNormalChar(x * 6, 0, textchar[x]); //top line
        clockDisplay->putNormalChar(x * 6, 8, textchar[x + 8]); //bottom line
      }
    }
    delay(50);
  }
  LEAVE("NormalClockMode");
}

#endif
