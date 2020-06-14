#ifndef _UserInterfaceInput_CPP_
#define _UserInterfaceInput_CPP_

#include <Arduino.h>

#include "UserInterfaceInput.hpp"

UserInterfaceInput::UserInterfaceInput():
  buttonA(Button(pinButtonA, BUTTON_PULLUP)), // Setup button A (using button library)
  buttonB(Button(pinButtonB, BUTTON_PULLUP)) // Setup button B (using button library)
{
  // empty
}

void UserInterfaceInput::setup() {
  pinMode(pinButtonA, INPUT_PULLUP);
  pinMode(pinButtonB, INPUT_PULLUP);
  digitalWrite(pinButtonA, HIGH);                 // turn on pullup resistor for button
  digitalWrite(pinButtonB, HIGH);                 // turn on pullup resistor for button
}

bool UserInterfaceInput::isAnyButtonPressed() {
  return isButtonAPressed() || isButtonBPressed();
}

bool UserInterfaceInput::isButtonAPressed() {
  return buttonA.uniquePress();
}

bool UserInterfaceInput::isButtonBPressed() {
  return buttonB.uniquePress();
}

#endif
