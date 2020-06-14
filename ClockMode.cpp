#ifndef _ClockMode_CPP_
#define _ClockMode_CPP_

#include <Arduino.h>
#include "ClockMode.hpp"

ClockMode::ClockMode(const String& name):
  name(name)
{}

void ClockMode::setNext(ClockMode *nextMode) {
  next = nextMode;
}

ClockMode* ClockMode::getNext() {
  return next;
}

String ClockMode::getName() {
  return name;
}

#endif
