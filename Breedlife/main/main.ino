#include <Arduino.h>

#include "acs712.h"
#include "ds1307.h"
#include "ntc10k.h"
#include "sdcard.h"
#include "lcd1604.h"
#include "sim7600e.h"
#include "ledbutton.h"

//ACS712_Sensor ACS712(VIN, SEN, QV, MOE);

////note  old version 0X27, new version 0x3F
//LDC1604Display LDC1604(0x27, 16, 4);

//DS1307Time DS1307;

NTC10kSensor NTC10k;

//SDCard  SDMemory;

bool  BUTTON1_Flag=false, BUTTON2_Flag=false, BUTTON3_Flag=false, BUTTON4_Flag=false;
char s[10];

void setup()
{
  Serial.begin(9600);

//  // initial lcd
//  LDC1604.init();                    
//  LDC1604.backlight();
  
//  // LED-Buzz-Butotn
//  Init_Buzz();
//  Init_LED();
//  Init_Button();

//  // RTC-DS1307
//  DS1307.RTC_Init();

//  // SD-Card
//  SDMemory.SD_Init();
  
//  // NTC10k
//  NTC10k.NTC10k_ReadTemperture(NTC10kpin1);
}

void loop()
{
//  Serial.print("analog:");
//  Serial.println(analogRead(A2));
//    
//  Serial.print("Current:");
//  Serial.println(ACS712.getCurrent(ACS1), 3);
//
//  Serial.print("Average:");
//  Serial.println(ACS712.getCurrentAverage(ACS1, 10), 3);
//  delay(2000);
}
