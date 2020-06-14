#ifndef _DigitsClockMode_HPP_
#define _DigitsClockMode_HPP_

#include "AbstractClockMode.hpp"

/**
   Show the time in 10x14 characters and update it whilst the loop runs.
*/
class DigitsClockMode : public AbstractClockMode {
  public:
    DigitsClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();
  private:
    void drawColon(bool mode, int offset);
};

DigitsClockMode::DigitsClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}


void DigitsClockMode::action() {
  ENTER("DigitsClockMode");
  bool ampm = clockDisplay->getConfigurationAmPm();

  clockDisplay->cls();

  char buffer[3];   //for int to char conversion to turn rtc values into chars we can print on screen
  byte offset = 0;  //used to offset the x postition of the digits and centre the display when we are in 12 hour mode and the clock shows only 3 digits. e.g. 3:21

  //do 12/24 hour conversion if ampm set to 1
  byte hours = clockDisplay->getClock()->getHours();
  if (hours > 12) {
    hours = hours - ampm * 12;
  }
  if (hours < 1) {
    hours = hours + ampm * 12;
  }

  //set the next minute we show the date at
  clockDisplay->set_next_date();

  // initially set mins to value 100 - so it wll never equal rtc[1] on the first loop of the clock, meaning we draw the clock display when we enter the function
  byte secs = 100;
  byte mins = 100;
  int count = 0;

  //run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    //check whether it's time to automatically display the date
    clockDisplay->check_show_date();

    //draw the flashing : as on if the secs have changed.
    if (secs != clockDisplay->getClock()->getSeconds()) {
      secs = clockDisplay->getClock()->getSeconds();
      drawColon(1, offset);
      count = 500;
    }

    //if count has run out, turn off the :
    if (count == 0) {
      drawColon(0, offset);
    } else {
      count--;
    }

    //re draw the display if mins != rtc[1] i.e. if the time has changed from what we had stored in mins, (also trigggered on first entering function when mins is 100)
    if (mins != clockDisplay->getClock()->getMinutes()) {

      //update mins and hours with the new values
      mins = clockDisplay->getClock()->getMinutes();
      hours = clockDisplay->getClock()->getHours();

      formatHours(buffer, hours, ampm);
      //print hours
      //if we in 12 hour mode and hours < 10, then don't print the leading zero, and set the offset so we centre the display with 3 digits.
      if (ampm && hours < 10) {
        offset = 5;

        //if the time is 1:00am clear the entire display as the offset changes at this time and we need to blank out the old 12:59
        if ((hours == 1 && mins == 0) ) {
          clockDisplay->cls();
        }
      } else {
        //else no offset and print hours tens digit
        offset = 0;

        //if the time is 10:00am clear the entire display as the offset changes at this time and we need to blank out the old 9:59
        if (hours == 10 && mins == 0) {
          clockDisplay->cls();
        }
        clockDisplay->putBigChar(0, 1, buffer[0]);
      }
      //print hours ones digit
      clockDisplay->putBigChar(12 - offset, 1, buffer[1]);

      //print minutes
      formatNumber(buffer, mins);
      clockDisplay->putBigChar(26 - offset, 1, buffer[0]);
      clockDisplay->putBigChar(38 - offset, 1, buffer[1]);
      clockDisplay->display();
    }
    delay(1);
  }
  LEAVE("DigitsClockMode");
}

void DigitsClockMode::drawColon(bool mode, int offset) {
  // top point
  clockDisplay->setPixel(23 - offset, 4, mode);
  clockDisplay->setPixel(23 - offset, 5, mode);
  clockDisplay->setPixel(24 - offset, 4, mode);
  clockDisplay->setPixel(24 - offset, 5, mode);
  // bottom point
  clockDisplay->setPixel(23 - offset, 10, mode);
  clockDisplay->setPixel(23 - offset, 11, mode);
  clockDisplay->setPixel(24 - offset, 10, mode);
  clockDisplay->setPixel(24 - offset, 11, mode);
  clockDisplay->display();
}

#endif
