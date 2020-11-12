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


// chua viet ham doc dien ap
#define VOLTAGE     A12

/*
 *  File này sẽ gọi các module vào nha
*/
unsigned long t1, t2, t3;

float SOH(void);

// Depth of Discharge - Độ sâu phóng điện
unsigned long DOD(void);

float getVoltage(void);

#endif
