#ifndef _UserInterfaceInput_CPP_
#define _UserInterfaceInput_CPP_

#include <Arduino.h>

#include "UserInterfaceInput.hpp"

UserInterfaceInput::UserInterfaceInput():
  buttonA(Button(pinButtonA)), // Setup button A (using button library)
  buttonB(Button(pinButtonB)) // Setup button B (using button library)
{
  // empty
}

void UserInterfaceInput::setup() {
  buttonA.begin();
  buttonB.begin();
}

bool UserInterfaceInput::isAnyButtonPressed() {
  return isButtonAPressed() || isButtonBPressed();
}

bool UserInterfaceInput::isButtonAPressed() {
  return buttonA.pressed();
}

bool UserInterfaceInput::isButtonBPressed() {
  return buttonB.pressed();
}

#endif
