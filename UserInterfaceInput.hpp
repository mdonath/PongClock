#ifndef _UserInterfaceInput_HPP_
#define _UserInterfaceInput_HPP_

#include <Button.h>

class UserInterfaceInput {
  public:
    UserInterfaceInput();
    void setup();

    bool isAnyButtonPressed();
    bool isButtonAPressed();
    bool isButtonBPressed();

  private:
    const int pinButtonA = 21;
    const int pinButtonB = 22;

    Button buttonA;
    Button buttonB;
};

#endif
