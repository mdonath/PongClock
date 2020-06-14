#ifndef _JumbleClockMode_HPP_
#define _JumbleClockMode_HPP_

#include "ClockMode.hpp"

/**
   like normal but with slide effect
   show time and date and use a random jumble of letters transition each time the time changes.
*/
class JumbleClockMode : public AbstractClockMode {
  public:
    JumbleClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();
  private:
    void jumble();

    void updateSeconds(byte secs);

    void randomizeSequence(byte *seq, int length);
    void randomizeCounters(byte* counter, int length);
    void fillEndchar(char* endchar);
};

JumbleClockMode::JumbleClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}


/**
   Action for Jumble.
*/
void JumbleClockMode::action() {
  ENTER("JumbleClockMode");
  byte mins = 100;
  byte secs = 100;

  clockDisplay->cls();

  //run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    //if secs changed then update them on the display
    byte newSecs = clockDisplay->getClock()->getSeconds();
    if (secs != newSecs) {
      updateSeconds(secs);
      secs = newSecs;
    }

    // if minute changes or show runs out, do the jumble effect
    byte newMins = clockDisplay->getClock()->getMinutes();
    if (mins != newMins) {
      jumble();
      mins = newMins;
    }
    delay(50);
  }
  LEAVE("JumbleClockMode");
}

void JumbleClockMode::updateSeconds(byte secs) {
  char buffer[3];
  formatNumber(buffer, secs);
  clockDisplay->putNormalChar( 30, 0, ':'); //seconds colon
  clockDisplay->putNormalChar( 36, 0, buffer[0]); //seconds
  clockDisplay->putNormalChar( 42, 0, buffer[1]); //seconds
}



void JumbleClockMode::jumble() {

  const char allchars[63] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"
  }; //chars to show as part of jumble effect

  byte seq[16];
  // 16 counters - one for each character on the display. These each count down from a random number to 0. When they hit 0 the end charater is displayed
  byte counter[16]; 
  
  randomizeSequence(seq, 16);
  randomizeCounters(counter, 16);

  // the 16 end characters that the clock settles on
  char endchar[16];
  fillEndchar(endchar);

  byte x = 0;
  byte y = 0;

  // until all counters are 0
  byte alldone = 0;
  while (alldone < 16) {

    // for each char
    for (byte c = 0; c < 16 ; c++) {

      // update endchar for seconds during jumble effect - as they may change.
      clockDisplay->getClock()->updateTime();

      char buffer[3];
      formatNumber(buffer, clockDisplay->getClock()->getSeconds());
      endchar[6] = buffer[0];
      endchar[7] = buffer[1];

      if (seq[c] < 8) {
        x = 0;
        y = 0;
      }
      else {
        x = 8;
        y = 8;
      }

      // if counter > 1 then put random char
      if (counter[ seq[c] ] > 0) {
        clockDisplay->putNormalChar( ( seq[c] - x) * 6, y, allchars[random(0, 63)]); //random
        counter[seq[c]]--;
      }

      // if counter == 1 then put final char, the set counter to 0. As we don't want to trigger this 'if' again as we don't want to keep inc'ing alldone.
      if (counter[seq[c]] == 1) {
        clockDisplay->putNormalChar( (seq[c] - x) * 6, y, endchar[seq[c]]); //final char
        counter[seq[c]] = 0; //set counter to 0
        alldone++; //increment done tally of counters
      }

      // if counter == 0 then print the char again but don't inc alldone. The only chars that should change are the seconds.
      if (counter[seq[c]] == 0) {
        clockDisplay->putNormalChar( (seq[c] - x) * 6, y, endchar[seq[c]]);
      }

      if (clockDisplay->getUserInterfaceInput()->isButtonAPressed()) {
        clockDisplay->switchModeMenu();
        return;
      }
      delay(5);
    }
  }
}

void JumbleClockMode::randomizeSequence(byte *seq, int length) {
  // fill an arry with 0-15 and randomize the order so we can plot letters in a jumbled pattern rather than sequentially
  for (int i = 0; i < length; i++) {
    seq[i] = i;  // fill the array in order
  }
  // randomise out array of numbers
  for (int i = 0; i < (length - 1); i++) {
    int r = i + (rand() % (length - i)); // Random remaining position.
    int temp = seq[i];
    seq[i] = seq[r];
    seq[r] = temp;
  }
}

void JumbleClockMode::randomizeCounters(byte* counter, int length) {
  //set counters to random numbers between 3 - 23. They show a random character until they get to 0, then the correct end char is shown
  for (byte c = 0; c < length ; c++) {
    counter[c] = 3 + random (0, 20);
  }
}

/**
   Fill lines with desired end result.
*/
void JumbleClockMode::fillEndchar(char* endchar) {
  char buffer[3];

  formatHours(buffer, clockDisplay->getClock()->getHours(), clockDisplay->getConfigurationAmPm());
  endchar[0] = buffer[0];
  endchar[1] = buffer[1];
  endchar[2] = ':';
  fillResult(endchar,  3, 2, clockDisplay->getClock()->getMinutes());
  endchar[5] = ':';
  fillResult(endchar,  8, 2, clockDisplay->getClock()->getDay());
  endchar[10] = '-';
  fillResult(endchar, 11, 2, clockDisplay->getClock()->getMonth());
  endchar[13] = '-';
  fillResult(endchar, 14, 2, clockDisplay->getClock()->getYear() % 100);
}


#endif
