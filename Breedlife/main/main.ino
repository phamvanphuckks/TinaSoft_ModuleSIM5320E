#include <Arduino.h>
#include "App.h"

float PX=0.0;
void setup()
{
  Serial.begin(9600);
  
  DS1307_Init(); // RTC-DS1307
  ECHOLN("DS1307");
  SDCard_Init(); // SD-Card
  ECHOLN("SD INNIT");
  SIM_Timer_Init();
  Led_Button_Buzz_Init(); // LED-Buzz-Butotn
  ECHOLN("BUZZ");
  LCD1604_Init(); // initial lcd
  ECHOLN("LCD");
  SIM7600E_Init(); // Module SIM 
}

void loop()
{
//  long t1, t2;
//  Calulation_SOH_DOD(); 
//  DS1307_getTime(T1);
//  t1 = getTimeSOH(T1);
//  delay(12000);
//  DS1307_getTime(T2);
//  t2 = getTimeSOH(T2);
//  getUIdischarge();
//  PX += getSOH(t1, t2);
//  ECHOLN(PX);
  /*
   * Xử lý LCD - button
  */
  move_up();
  move_down();
  move_back();
  move_enter();
  
  if((lcdFlag == BAT_Flag) && TIM_UpdateLCD > 10)   // 5s update - battery
  {
    LCD_battery(pos_Batt+1);
    TIM_UpdateLCD = 0;
  }
  if((lcdFlag == HOME_Flag) && TIM_UpdateLCD > 10)  // 5s update - home
  {
    LCD_home();
    TIM_UpdateLCD = 0;
  }  

  /* 
   * module SIM7600E - 1p truy vấn lại
   */
  SIM_UpDate(10);
//  SIM_Synch(3600);
}
