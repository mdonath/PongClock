#ifndef _ScrollerClockMode_HPP_
#define _ScrollerClockMode_HPP_

#include "ClockMode.hpp"

/**
   Scrolls a text.
*/
class ScrollerClockMode : public AbstractClockMode {
  public:
    ScrollerClockMode(const String& name, ClockDisplay* clockDisplay);

    void action();

    void setText(const String& text);

  private:
    void scroll();

    String message;
    Text* text;
    int steps = 0;
};

ScrollerClockMode::ScrollerClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // empty
}


/**
   Action for Jumble.
*/
void ScrollerClockMode::action() {
  ENTER("ScrollerClockMode");

  clockDisplay->cls();

  //run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {
    scroll();
    if (steps <= 0) {
      steps = 48+8*message.length();
      text->setX(steps);
    }
    delay(50);
  }
  LEAVE("ScrollerClockMode");
}

void ScrollerClockMode::setText(const String& msg) {
//  if (message) {
//    delete message;
//  }
  message = msg;

  if (text) {
    delete text;
  }
  text = new Text(48,0, message, clockDisplay->lmd);
  steps = 48+ 8 * message.length();
}

void ScrollerClockMode::scroll() {
  text->draw();
  text->nextStep();
  steps--;
}

#endif
