#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include <avr/interrupt.h>
#include "AppDebug.h"

#include "sim7600e.h"
#include "ledbutton.h"

extern volatile uint8_t  SIM_Flag;
extern volatile uint16_t Sync_Flag;
extern volatile uint16_t TIM_UpdateCurrent;
extern volatile bool BT_UP_Flag, BT_DOWN_Flag, BT_BACK_Flag, BT_ENTER_Flag; 

void BT_Timer_Init(void);
void SIM_Timer_Init(void);

#endif
