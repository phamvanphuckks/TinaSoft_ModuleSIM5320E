#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include <avr/interrupt.h>
#include "AppDebug.h"
#include "App.h"

#include "sim7600e.h"
#include "ledbutton.h"

extern volatile uint16_t  SIM_Flag, Sync_Flag, TIM_SOH_Flag, TIM_UpdateLCD;
extern volatile bool      BT_UP_Flag, BT_DOWN_Flag, BT_BACK_Flag, BT_ENTER_Flag; 

void BT_Timer_Init(void);
void SIM_Timer_Init(void);

#endif
