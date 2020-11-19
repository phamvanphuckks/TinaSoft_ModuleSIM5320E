/**
 * Author: Tinasoft.vn
 */
#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>

#include "acs712.h"
#include "ds1307.h"
#include "ntc10k.h"
#include "sdcard.h"
#include "lcd1604.h"
#include "sim7600e.h"
#include "ledbutton.h"
#include "MyJson.h"

//  MyJsonClass MyJson;
//  MyJson.encode("temperture", 5);
//  MyJson.encode("humidity", 7);
//  MyJson.encode("light", 8);
//
//  String message = MyJson.getStringEncode();
//  Serial.println(message);
//  char res[200];
//  message.toCharArray(res, 200);
//  Serial.println(message);

#define VOLTAGE     A12

/*
 *  File này sẽ gọi các module vào nha
*/

float getSOH(unsigned long t1, unsigned long t2, float u, float i);
float getVoltageOfCircuit(void);
float getAmpleOfCircuit(void);

unsigned long getDOD(unsigned long t1, unsigned long t2);

#endif /* __APP_H__ */
