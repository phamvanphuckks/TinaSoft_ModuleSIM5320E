#include "ds1307.h"

void DS1307Time::RTC_Init(void)
{
  if (!this->RTC.begin())
  {
    ECHOLN("Couldn't find RTC");
  }
  
  if (!this->RTC.isrunning())
  {
    ECHOLN("RTC is NOT running!");
  }
  
  this->RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // get time from PC 
  //this->RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // manual 
}

char DS1307Time::RTC_getTime(char* time)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(time,"%d:%d:%d", RTC_Datetime.hour(), RTC_Datetime.minute(), RTC_Datetime.second());
  ECHOLN(time);
}

char DS1307Time::RTC_getDate(char* date)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(date,"%d:%d:%d", RTC_Datetime.year(), RTC_Datetime.month(), RTC_Datetime.day());
  ECHOLN(date);
}

char DS1307Time::RTC_getDateTime(char* datetime)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(datetime,"%d:%d:%d,%d:%d:%d", RTC_Datetime.hour(), RTC_Datetime.minute(), RTC_Datetime.second(), RTC_Datetime.year(), RTC_Datetime.month(), RTC_Datetime.day());
  ECHOLN(datetime);
}
    
