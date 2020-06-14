#ifndef _AbstractClockMode_HPP_
#define _AbstractClockMode_HPP_

#include "ClockDisplay.hpp"

#define ENTER(msg) Serial.print(msg); Serial.println("::action - enter");
#define LEAVE(msg) Serial.print(msg); Serial.println("::action - leave");

/**
   Abstract class for ClockModes.
*/
class AbstractClockMode : public ClockMode {
  public:
    AbstractClockMode(const String& name, ClockDisplay* clockDisplay);
    virtual void action() = 0;

    const char* getWeekdayShort(uint8_t weekday);
    const char* getWeekdayFull(uint8_t weekday);
    const char* getMonthFull(uint8_t month);
    const char* getSuffix(uint8_t index);
    const char* getSuffixText(int date);

    void formatNumber(char* buffer, int number);
    void formatHours(char* buffer, int hours, boolean ampm);


    void fillResult(char* result, int start, int length, int value);

    char daysfull[7][9] = {
      "Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdg", "Vrijdag", "Zaterdag"
    };

    //day array
    char days[7][3] = {
      "Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"
    };

    //date suffix array, used in slide, normal and jumble modes. e,g, 1st 2nd ...
    char suffix[4][3] = {
      "e ", "e ", "e ", "e "
    };

    char monthshort[12][4] = {
      "-01", "-02", "-03", "-04", "-05", "-06", "-07", "-08", "-09", "-10", "-11", "-12"
    };

    //array of month names to print on the display. Some are shortened as we only have 8 characters across to play with
    char monthnames[12][9] = {
      "Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli", "Augustus", "Septmber", "Oktober", "November", "December"
    };

  protected:
    ClockDisplay* clockDisplay;
    void centerAndPlot(const char *str_line, int y);

};

AbstractClockMode::AbstractClockMode(const String& name, ClockDisplay* clockDisplay):
  ClockMode(name),
  clockDisplay(clockDisplay)
{
  // empty
}

void AbstractClockMode::centerAndPlot(const char *str_line, int y) {
  //work out offset to center top line on display.
  byte len = 0;
  //get length of message
  while (str_line[len]) {
    len++;
  };
  byte offset_line = (X_MAX - ((len - 1) * 4)) / 2;

  // plot top line
  byte i = 0;
  while (str_line[i]) {
    clockDisplay->putTinyChar((i * 4) + offset_line, y, str_line[i]);
    i++;
  }
}

void AbstractClockMode::fillResult(char* result, int start, int length, int value) {
  char buffer[3];
  formatNumber(buffer, value);
  for (int i = 0; i < length && buffer[i]; i++) {
    result[start + i] = buffer[i];
  }
}

void AbstractClockMode::formatNumber(char* buffer, int number) {
  itoa (number, buffer, 10);

  if (number < 10) {
    buffer[1] = buffer[0];
    buffer[0] = '0';
  }
}

void AbstractClockMode::formatHours(char* buffer, int hours, boolean ampm) {
  if (hours > 12) {
    hours = hours - ampm * 12;
  }
  if (hours < 1) {
    hours = hours + ampm * 12;
  }
  formatNumber(buffer, hours);

  if (hours < 10 && ampm) {
    //if we are in 12 hour mode blank the leading zero.
    buffer[0] = ' ';
  }
}


const char* AbstractClockMode::getWeekdayShort(uint8_t weekday) {
  return days[weekday];
}
const char* AbstractClockMode::getWeekdayFull(uint8_t weekday) {
  return daysfull[weekday];
}
const char* AbstractClockMode::getMonthFull(uint8_t month) {
  return monthnames[month];
}
const char* AbstractClockMode::getSuffix(uint8_t index) {
  return suffix[index];
}
const char* AbstractClockMode::getSuffixText(int date) {
  byte s = 3;
  if (date == 1 || date == 21 || date == 31) {
    s = 0;
  }
  else if (date == 2 || date == 22) {
    s = 1;
  }
  else if (date == 3 || date == 23) {
    s = 2;
  }
  return getSuffix(s);
}
#endif
