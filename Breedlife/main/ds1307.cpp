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
  
  this->RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // get time from PC- thời gian lần lưu file gần nhất 
  //this->RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // manual 
}

char DS1307Time::RTC_getTime(char* time)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(time,"%0.2d:%0.2d:%0.2d", RTC_Datetime.hour(), RTC_Datetime.minute(), RTC_Datetime.second());
  ECHOLN(time);
}

void DS1307Time::RTC_getTime(int *t_hour, int *t_minute, int *t_second)
{
  this->RTC_Datetime = this->RTC.now();
  t_hour   = RTC_Datetime.hour();
  t_minute = RTC_Datetime.minute();
  t_second = RTC_Datetime.second();
}

char DS1307Time::RTC_getDate(char* date)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(date,"%d:%0.2d:%0.2d", RTC_Datetime.year(), RTC_Datetime.month(), RTC_Datetime.day());
  ECHOLN(date);
}

char DS1307Time::RTC_getDateTime(char* datetime)
{
  this->RTC_Datetime = this->RTC.now();
  sprintf(datetime,"%0.2d:%0.2d:%0.2d,%d:%0.2d:%0.2d", RTC_Datetime.hour(), RTC_Datetime.minute(), RTC_Datetime.second(), RTC_Datetime.year(), RTC_Datetime.month(), RTC_Datetime.day());
  ECHOLN(datetime);
}
  
