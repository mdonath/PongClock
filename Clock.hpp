#ifndef _Clock_HPP_
#define _Clock_HPP_

#include <Arduino.h>
#include <time.h>

class Clock {
  public:
    Clock();
    
    int* updateTime();

    int getSeconds();
    int getMinutes();
    int getHours();
    int getDayOfWeek();
    int getDay();
    int getMonth();
    int getYear();


  private:
    int rtc[7];
};

#endif
