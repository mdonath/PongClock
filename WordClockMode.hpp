#ifndef _WordClockMode_HPP_
#define _WordClockMode_HPP_

#include "AbstractWordClockMode.hpp"

/**
      Show the time in english words.
*/
class WordClockMode : public AbstractWordClockMode {
  public:
    WordClockMode(const String& name, ClockDisplay* clockDisplay);

  private:
    void fillLines(byte hours, byte minutes, char *str_top, char *str_bot);

    const char numbers[19][10]   = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
      "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
    };
    const char numberstens[5][7] = {
      "ten", "twenty", "thirty", "forty", "fifty"
    };

};

WordClockMode::WordClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractWordClockMode(name, clockDisplay)
{
  // empty
}

void WordClockMode::fillLines(byte hours, byte minutes, char *str_top, char *str_bot) {
  bool ampm = clockDisplay->getConfigurationAmPm();
  if (hours > 12) {
    hours = hours - ampm * 12;
  }
  if (hours < 1) {
    hours = hours + ampm * 12;
  }
  //make hours into 12 hour format
  if (hours > 12) {
    hours = hours - 12;
  }
  if (hours == 0) {
    hours = 12;
  }

  //split mins value up into two separate digits
  int minsdigit = minutes % 10;
  byte minsdigitten = (minutes / 10) % 10;

  if (minutes < 10) {
    // if mins <= 10 , then top line has to read "minsdigit past" and bottom line reads hours
    strcpy (str_top, numbers[minsdigit - 1]);
    strcat (str_top, " PAST");
    strcpy (str_bot, numbers[hours - 1]);
  }

  if (minutes == 10) {
    // if mins = 10, cant use minsdigit as above, so special case to print 10 past /n hour.
    strcpy (str_top, numbers[9]);
    strcat (str_top, " PAST");
    strcpy (str_bot, numbers[hours - 1]);

  } else if (minsdigitten != 0 && minsdigit != 0) {
    //if time is not on the hour - i.e. both mins digits are not zero,
    //then make top line read "hours" and bottom line read "minstens mins" e.g. "three /n twenty one"

    strcpy (str_top, numbers[hours - 1]);

    //if mins is in the teens, use teens from the numbers array for the bottom line, e.g. "three /n fifteen"
    if (minutes >= 11 && minutes <= 19) {
      strcpy (str_bot, numbers[minutes - 1]);

      //else bottom line reads "minstens mins" e.g. "three \n twenty three"
    } else {
      strcpy (str_bot, numberstens[minsdigitten - 1]);
      strcat (str_bot, " ");
      strcat (str_bot, numbers[minsdigit - 1]);
    }
  } else if (minsdigitten != 0 && minsdigit == 0  ) {
    // if mins digit is zero, don't print it. read read "hours" "minstens" e.g. "three /n twenty"
    strcpy (str_top, numbers[hours - 1]);
    strcpy (str_bot, numberstens[minsdigitten - 1]);

  } else if (minsdigitten == 0 && minsdigit == 0  ) {
    //if both mins are zero, i.e. it is on the hour, the top line reads "hours" and bottom line reads "o'clock"
    strcpy (str_top, numbers[hours - 1]);
    strcpy (str_bot, "O'CLOCK");
  }
}

#endif
