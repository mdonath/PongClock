#ifndef _Clock_CPP_
#define _Clock_CPP_

#include "Clock.hpp"

Clock::Clock() {
  // NOP
}

int* Clock::updateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return rtc;
  }
  rtc[0] = timeinfo.tm_sec; // seconds
  rtc[1] = timeinfo.tm_min; // minutes
  rtc[2] = timeinfo.tm_hour; // hours
  rtc[3] = timeinfo.tm_wday; // day of week (0-6, 0 = Sun)
  rtc[4] = timeinfo.tm_mday; // date
  rtc[5] = timeinfo.tm_mon + 1; // month
  rtc[6] = timeinfo.tm_year + 1900; // year

  // flash arduino led on pin 13 every second
  if ( (rtc[0] % 2) == 0) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
  return rtc;
}

int Clock::getSeconds() {
  return rtc[0];
}
int Clock::getMinutes() {
  return rtc[1];
}
int Clock::getHours() {
  return rtc[2];
}
int Clock::getDayOfWeek() {
  return rtc[3];
}
int Clock::getDay() {
  return rtc[4];
}
int Clock::getMonth() {
  return rtc[5];
}
int Clock::getYear() {
  return rtc[6];
}

#endif
