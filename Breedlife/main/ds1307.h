#ifndef __DS_1307__
#define __DS_1307__

#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"
#include "AppDebug.h"

class DS1307Time
{
  public:
    void RTC_Init(void);
    char RTC_getDateTime(char* datetime);
    char RTC_getDate(char* date);
    char RTC_getTime(char* time);
    void RTC_getTime(int *t_hour, int *t_minute, int *t_second);
  private:
    RTC_DS1307 RTC;
    DateTime   RTC_Datetime;
};


#endif
