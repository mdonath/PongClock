#ifndef _DutchWordClockMode_HPP_
#define _DutchWordClockMode_HPP_

#include "AbstractWordClockMode.hpp"

/**
   Show the time in 10x14 characters and update it whilst the loop runs.
*/
class DutchWordClockMode : public AbstractWordClockMode {
  public:
    DutchWordClockMode(const String& name, ClockDisplay* clockDisplay);

  private:
    void fillLines(byte hours, byte minutes, char *str_top, char *str_bot);

    const  char numbers[20][10]   = {
      "twaalf", "een", "twee", "drie", "vier", "vijf", "zes", "zeven", "acht", "negen", "tien",
      "elf", "twaalf", "dertien", "veertien", "vijftien", "zestien", "zeventien", "achttien", "negentien"
    };
};

DutchWordClockMode::DutchWordClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractWordClockMode(name, clockDisplay)
{
  // empty
}


void DutchWordClockMode::fillLines(byte hours, byte minutes, char *str_top, char *str_bot) {
  if (hours == 0 && minutes == 0) {
    strcpy(str_top, "het is");
    strcpy(str_bot, "middernacht");
    return;
  }

  hours = (hours == 0) ? 12 : hours > 12 ? hours - 12 : hours;
  byte next_hours = (hours % 12) + 1;

  if (minutes == 0) {                         // heel uur
    strcpy(str_top, "het is ");
    strcpy(str_bot, numbers[hours]);
    strcat(str_bot, " uur");
  } else if (minutes >= 1 && minutes < 14) {  // 1-13 minuten
    strcpy(str_top, numbers[minutes]);
    strcat(str_top, " over");
    strcpy(str_bot, numbers[hours]);
  } else if (minutes == 15) {                 // 15 minuten
    strcpy(str_top, "kwart over");
    strcpy(str_bot, numbers[hours]);
  } else if (minutes >= 14 && minutes < 20) { // 14-19 minuten
    strcpy(str_top, numbers[minutes]);
    strcpy(str_bot, "over ");
    strcat(str_bot, numbers[hours]);
  } else if (minutes >= 20 && minutes < 30) { // 19-29 minuten
    strcpy(str_top, numbers[30 - minutes]);
    strcat(str_top, " voor");
    strcpy(str_bot, "half ");
    strcat(str_bot, numbers[next_hours]);
  } else if (minutes == 30) {                 // 30 minuten
    strcpy(str_top, "half");
    strcpy(str_bot, numbers[next_hours]);
  } else if (minutes >= 31 && minutes < 44) { // 31-43 minuten
    strcpy(str_top, numbers[minutes - 30]);
    strcat(str_top, " over");
    strcpy(str_bot, "half ");
    strcat(str_bot, numbers[next_hours]);
  } else if (minutes == 45) {                 // 45 minuten
    strcpy(str_top, "kwart voor");
    strcpy(str_bot, numbers[next_hours]);
  } else if (minutes >= 44 && minutes < 60) { // 44-59 minuten
    strcpy(str_top, numbers[60 - minutes]);
    strcpy(str_bot, "voor ");
    strcat(str_bot, numbers[next_hours]);
  }
}

#endif
