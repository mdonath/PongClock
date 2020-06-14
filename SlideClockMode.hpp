#ifndef _SlideClockMode_HPP_
#define _SlideClockMode_HPP_

#include "AbstractClockMode.hpp"

// The time in milliseconds for the slide effect per character in slide mode. Make this higher for a slower effect
#define SLIDE_DELAY 20 

class SlideClockMode : public AbstractClockMode {
  public:
    SlideClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();
  private:
    void slideanim(byte x, byte y, byte sequence, char current_c, char new_c);
};

SlideClockMode::SlideClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}

void SlideClockMode::action() {
  ENTER("SlideClockMode");

  bool ampm = clockDisplay->getConfigurationAmPm();

  byte digits_old[10] = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99
  }; //old values  we store time in. Set to somthing that will never match the time initially so all digits get drawn wnen the mode starts

  byte digits_new[10]; //new digits time will slide to reveal
  byte digits_x_pos[10] = {
    42, 36, 24, 18, 6, 0, 24, 18, 42, 36
  }; //x pos for which to draw each digit at - last 2 are bottom line

  char old_char[2]; //used when we use itoa to transpose the current digit (type byte) into a char to pass to the animation function
  char new_char[2]; //used when we use itoa to transpose the new digit (type byte) into a char to pass to the animation function

  //old_chars - stores the 5 day and date suffix chars on the display. e.g. "mon" and "st". We feed these into the slide animation as the current char when these chars are updated.
  //We sent them as A initially, which are used when the clocl enters the mode and no last chars are stored.
  char old_chars[6] = "AAAAA";

  //plot the clock colons on the display - these don't change
  clockDisplay->cls();
  clockDisplay->putNormalChar( 12, 0, ':');
  clockDisplay->putNormalChar( 30, 0, ':');
  clockDisplay->putNormalChar( 30, 8, '-');

  byte old_secs = 100; //store seconds in old_secs. We compare secs and old secs. WHen they are different we redraw the display

  // run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    // if secs have changed then update the display
    if (clockDisplay->getClock()->getSeconds() != old_secs) {
      old_secs = clockDisplay->getClock()->getSeconds();

      //do 12/24 hour conversion if ampm set to 1
      byte hours = clockDisplay->getClock()->getHours();
      if (hours > 12) {
        hours = hours - ampm * 12;
      }
      if (hours < 1) {
        hours = hours + ampm * 12;
      }

      //split all date and time into individual digits - stick in digits_new array

      //clockDisplay->getSeconds() = secs                        //array pos and digit stored
      digits_new[0] = (clockDisplay->getClock()->getSeconds() % 10);         //0 - secs ones
      digits_new[1] = ((clockDisplay->getClock()->getSeconds() / 10) % 10);  //1 - secs tens
      //clockDisplay->getMinutes() = mins
      digits_new[2] = (clockDisplay->getClock()->getMinutes() % 10);         //2 - mins ones
      digits_new[3] = ((clockDisplay->getClock()->getMinutes() / 10) % 10);  //3 - mins tens
      //clockDisplay->getHours() = hours
      digits_new[4] = (hours % 10);         //4 - hour ones
      digits_new[5] = ((hours / 10) % 10);  //5 - hour tens
      //clockDisplay->getDay() = date
      digits_new[6] = (clockDisplay->getClock()->getDay() % 10);         //6 - date ones
      digits_new[7] = ((clockDisplay->getClock()->getDay() / 10) % 10);  //7 - date tens
      //clockDisplay->getMonth() = month
      digits_new[8] = (clockDisplay->getClock()->getMonth() % 10);         //8 - month ones
      digits_new[9] = ((clockDisplay->getClock()->getMonth() / 10) % 10);  //9 - month tens

      //draw initial screen of all chars. After this we just draw the changes.

      //compare top line digits 0 to 5 (secs, mins and hours)
      for (byte i = 0; i <= 5; i++) {
        //see if digit has changed...
        if (digits_old[i] != digits_new[i]) {

          //run sequence for each in turn
          for (byte seq = 0; seq <= 8 ; seq++) {

            //convert digit to string
            itoa(digits_old[i], old_char, 10);
            itoa(digits_new[i], new_char, 10);

            //if set to 12 hour mode and we're on digit 5 (hours tens mode) then check to see if this is a zero. If it is, blank it instead so we get 2.00pm not 02.00pm
            if (ampm && i == 5) {
              if (digits_new[5] == 0) {
                new_char[0] = ' ';
              }
              if (digits_old[5] == 0) {
                old_char[0] = ' ';
              }
            }
            //draw the animation frame for each digit
            slideanim(digits_x_pos[i], 0, seq, old_char[0], new_char[0]);
            delay(SLIDE_DELAY);
          }
        }
      }

      //compare date digit 6 (ones) and (7) tens - if either of these change we need to update the date line. We compare date tens as say from Jan 31 -> Feb 01 then ones digit doesn't change
      if ((digits_old[6] != digits_new[6]) || (digits_old[7] != digits_new[7])) {
        //change the day shown. Loop below goes through each of the 3 chars in turn e.g. "MON"
        for (byte day_char = 0; day_char <= 2 ; day_char++) {
          //run the anim sequence for each char
          for (byte seq = 0; seq <= 8 ; seq++) {
            //the day (0 - 6) Read this number into the days char array. the seconds number in the array 0-2 gets the 3 chars of the day name, e.g. m o n
            slideanim(6 * day_char, 8, seq, old_chars[day_char], getWeekdayShort(clockDisplay->getClock()->getDayOfWeek())[day_char]); //6 x day_char gives us the x pos for the char
            delay(SLIDE_DELAY);
          }
          //save the old day chars into the old_chars array at array pos 0-2. We use this next time we change the day and feed it to the animation as the current char. The updated char is fed in as the new char.
          old_chars[day_char] = getWeekdayShort(clockDisplay->getClock()->getDayOfWeek())[day_char];
        }

        // change the date tens digit (if needed) and ones digit. (the date ones digit wil alwaus change, but putting this in the 'if' loop makes it a bit neater code wise.)
        for (byte i = 7; i >= 6; i--) {
          if (digits_old[i] != digits_new[i]) {
            for (byte seq = 0; seq <= 8 ; seq++) {
              itoa(digits_old[i], old_char, 10);
              itoa(digits_new[i], new_char, 10);
              slideanim(digits_x_pos[i], 8, seq, old_char[0], new_char[0]);
              delay(SLIDE_DELAY);
            }
          }
        }
        // change the month tens digit (if needed) and ones digit. (the month ones digit wil alwaus change, but putting this in the 'if' loop makes it a bit neater code wise.)
        for (byte i = 9; i >= 8; i--) {
          if (digits_old[i] != digits_new[i]) {
            for (byte seq = 0; seq <= 8 ; seq++) {
              itoa(digits_old[i], old_char, 10);
              itoa(digits_new[i], new_char, 10);
              slideanim(digits_x_pos[i], 8, seq, old_char[0], new_char[0]);
              delay(SLIDE_DELAY);
            }
          }
        }
      }

      // save digits array to old for comparison next loop
      for (byte i = 0; i < 8; i++) {
        digits_old[i] =  digits_new[i];
      }
    }
  }
  LEAVE("SlideClockMode");
}

//called by slide
//this draws the animation of one char sliding on and the other sliding off. There are 8 steps in the animation, we call the function to draw one of the steps from 0-7
//inputs are are char x and y, animation frame sequence (0-7) and the current and new chars being drawn.
void SlideClockMode::slideanim(byte x, byte y, byte sequence, char current_c, char new_c) {

  //  To slide one char off and another on we need 9 steps or frames in sequence...

  //  seq# 0123456 <-rows of the display
  //   |   |||||||
  //  seq0 0123456  START - all rows of the display 0-6 show the current characters rows 0-6
  //  seq1  012345  current char moves down one row on the display. We only see it's rows 0-5. There are at display positions 1-6 There is a blank row inserted at the top
  //  seq2 6 01234  current char moves down 2 rows. we now only see rows 0-4 at display rows 2-6 on the display. Row 1 of the display is blank. Row 0 shows row 6 of the new char
  //  seq3 56 0123
  //  seq4 456 012  half old / half new char
  //  seq5 3456 01
  //  seq6 23456 0
  //  seq7 123456
  //  seq8 0123456  END - all rows show the new char

  //from above we can see...
  //currentchar runs 0-6 then 0-5 then 0-4 all the way to 0. starting Y position increases by 1 row each time.
  //new char runs 6 then 5-6 then 4-6 then 3-6. starting Y position increases by 1 row each time.

  //if sequence number is below 7, we need to draw the current char
  if (sequence < 7) {
    byte dots;
    // if (current_c >= 'A' &&  || (current_c >= 'a' && current_c <= 'z') ) {
    //   current_c &= 0x1F;   // A-Z maps to 1-26
    // }
    if (current_c >= 'A' && current_c <= 'Z' ) {
      current_c &= 0x1F;   // A-Z maps to 1-26
    }
    else if (current_c >= 'a' && current_c <= 'z') {
      current_c = (current_c - 'a') + 42;   // A-Z maps to 41-67
    }
    else if (current_c >= '0' && current_c <= '9') {
      current_c = (current_c - '0') + 32;
    }
    else if (current_c == ' ') {
      current_c = 0; // space
    }
    else if (current_c == '.') {
      current_c = 27; // full stop
    }
    else if (current_c == '\'') {
      current_c = 28; // single quote mark
    }
    else if (current_c == ':') {
      current_c = 29; //colon
    }
    else if (current_c == '>') {
      current_c = 30; // clock_mode selector arrow
    }

    byte curr_char_row_max = 6 - sequence; //the maximum number of rows to draw is 6 - sequence number
    byte start_y = sequence; //y position to start at - is same as sequence number. We inc this each loop

    //plot each row up to row maximum (calculated from sequence number)
    for (byte curr_char_row = 0; curr_char_row <= curr_char_row_max; curr_char_row++) {
      for (byte col = 0; col < 5; col++) {
        dots = myfont[(byte)current_c][col];
        if (dots & (64 >> curr_char_row))
          clockDisplay->plot(x + col, y + start_y, 1); //plot led on
        else
          clockDisplay->plot(x + col, y + start_y, 0); //else plot led off
      }
      start_y++;//add one to y so we draw next row one down
    }
  }

  //draw a blank line between the characters if sequence is between 1 and 7. If we don't do this we get the remnants of the current chars last position left on the display
  if (sequence >= 1 && sequence <= 7) {
    for (byte col = 0; col < 5; col++) {
      clockDisplay->plot(x + col, y + (sequence - 1), 0); //the y position to draw the line is equivalent to the sequence number - 1
    }
  }

  //if sequence is above 2, we also need to start drawing the new char
  if (sequence >= 2) {

    //work out char
    byte dots;
    //if (new_c >= 'A' && new_c <= 'Z' || (new_c >= 'a' && new_c <= 'z') ) {
    //  new_c &= 0x1F;   // A-Z maps to 1-26
    //}
    if (new_c >= 'A' && new_c <= 'Z' ) {
      new_c &= 0x1F;   // A-Z maps to 1-26
    }
    else if (new_c >= 'a' && new_c <= 'z') {
      new_c = (new_c - 'a') + 42;   // A-Z maps to 41-67
    }
    else if (new_c >= '0' && new_c <= '9') {
      new_c = (new_c - '0') + 32;
    }
    else if (new_c == ' ') {
      new_c = 0; // space
    }
    else if (new_c == '.') {
      new_c = 27; // full stop
    }
    else if (new_c == '\'') {
      new_c = 28; // single quote mark
    }
    else if (new_c == ':') {
      new_c = 29; // clock_mode selector arrow
    }
    else if (new_c == '>') {
      new_c = 30; // clock_mode selector arrow
    }

    byte newcharrowmin = 6 - (sequence - 2); //minimumm row num to draw for new char - this generates an output of 6 to 0 when fed sequence numbers 2-8. This is the minimum row to draw for the new char
    byte start_y = 0; //y position to start at - is same as sequence number. we inc it each row

    //plot each row up from row minimum (calculated by sequence number) up to 6
    for (byte newcharrow = newcharrowmin; newcharrow <= 6; newcharrow++) {
      for (byte col = 0; col < 5; col++) {
        dots = myfont[(byte)new_c][col];
        if (dots & (64 >> newcharrow))
          clockDisplay->plot(x + col, y + start_y, 1); //plot led on
        else
          clockDisplay->plot(x + col, y + start_y, 0); //else plot led off
      }
      start_y++;//add one to y so we draw next row one down
    }
  }
}

#endif
