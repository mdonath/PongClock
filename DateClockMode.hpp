#ifndef _DateClockMode_HPP_
#define _DateClockMode_HPP_

#include "AbstractClockMode.hpp"


class DateClockMode : public AbstractClockMode {
  public:
    DateClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();
};

DateClockMode::DateClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}

void DateClockMode::action() {
  ENTER("DateClockMode");
  clockDisplay->cls();

  byte dow = clockDisplay->getClock()->getDayOfWeek();
  byte date = clockDisplay->getClock()->getDay();
  byte month = clockDisplay->getClock()->getMonth() - 1;

  //call the flashing cursor effect for one blink at x,y pos 0,0, height 5, width 7, repeats 1
  clockDisplay->flashing_cursor(0, 0, 5, 7, 1);

  //print the day name
  int i = 0;
  while (daysfull[dow][i]) {
    clockDisplay->flashing_cursor(i * 6, 0, 5, 7, 0);
    clockDisplay->putNormalChar(i * 6 , 0, daysfull[dow][i]);
    i++;

    //check for button press and exit if there is one.
    if (clockDisplay->getUserInterfaceInput()->isAnyButtonPressed()) {
      return;
    }
  }

  //pause at the end of the line with a flashing cursor if there is space to print it.
  //if there is no space left, dont print the cursor, just wait.
  if (i * 6 < 48) {
    clockDisplay->flashing_cursor(i * 6, 0, 5, 7, 1);
  }
  else {
    delay(300);
  }

  //flash the cursor on the next line
  clockDisplay->flashing_cursor(0, 8, 5, 7, 0);

  //print the date on the next line: First convert the date number to chars so we can print it with putNormalChar
  char buffer[3];
  itoa(date, buffer, 10);

  //then work out date 2 letter suffix - eg st, nd, rd, th etc
  // char suffix[4][3]={"st", "nd", "rd", "th"  }; define at top of code
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

  //print the 1st date number
  clockDisplay->putNormalChar(0, 8, buffer[0]);

  //if date is under 10 - then we only have 1 digit so set positions of sufix etc one character nearer
  byte suffixposx = 6;

  //if date over 9 then print second number and set xpos of suffix to be 1 char further away
  if (date > 9) {
    suffixposx = 12;
    clockDisplay->flashing_cursor(6, 8, 5, 7, 0);
    clockDisplay->putNormalChar(6, 8, buffer[1]);
  }

  //print the 2 suffix characters
  clockDisplay->flashing_cursor(suffixposx, 8, 5, 7, 0);
  clockDisplay->putNormalChar(suffixposx, 8, suffix[s][0]);
  //delay(70);

  clockDisplay->flashing_cursor(suffixposx + 6, 8, 5, 7, 0);
  clockDisplay->putNormalChar(suffixposx + 6, 8, suffix[s][1]);
  //delay(70);

  //blink cursor after
  clockDisplay->flashing_cursor(suffixposx + 12, 8, 5, 7, 1);

  //replace day name with date on top line - effectively scroll the bottom line up by 8 pixels
  clockDisplay->cls();

  clockDisplay->putNormalChar(0, 0, buffer[0]);  //date first digit
  clockDisplay->putNormalChar(6, 0, buffer[1]);  //date second digit - this may be blank and overwritten if the date is a single number
  clockDisplay->putNormalChar(suffixposx, 0, suffix[s][0]);   //date suffix
  clockDisplay->putNormalChar(suffixposx + 6, 0, suffix[s][1]); //date suffix


  //flash the cursor for a second for effect
  clockDisplay->flashing_cursor(suffixposx + 12, 0, 5, 7, 0);

  //print the month name on the bottom row
  i = 0;
  while (monthnames[month][i]) {
    clockDisplay->flashing_cursor(i * 6, 8, 5, 7, 0);
    clockDisplay->putNormalChar(i * 6, 8, monthnames[month][i]);
    i++;

    //check for button press and exit if there is one.
    if (clockDisplay->getUserInterfaceInput()->isAnyButtonPressed()) {
      return;
    }
  }

  //blink the cursor at end if enough space after the month name, otherwise juts wait a while
  if (i * 6 < 48) {
    clockDisplay->flashing_cursor(i * 6, 8, 5, 7, 2);
  } else {
    delay(1000);
  }
  delay(3000);

  LEAVE("DateClockMode");
}


#endif
