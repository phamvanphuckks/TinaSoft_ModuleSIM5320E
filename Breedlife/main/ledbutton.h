#ifndef __LEDBUTTON_H__
#define __LEDBUTTON_H__

#include <Arduino.h>
#include <avr/interrupt.h>
#include "AppDebug.h"

// chú ý dùng timer đọc nút nhấn
#define BUTTON1 6
#define BUTTON2 7
#define BUTTON3 8
#define BUTTON4 9

#define BUZZ 23 

#define LED_RED     24
#define LED_YELLOW  25
#define LED_GREEN   26

extern bool  BUTTON1_Flag, BUTTON2_Flag, BUTTON3_Flag, BUTTON4_Flag; 

void Init_Buzz(void);
void Init_LED(void);
void Init_Button(void);
void readVoltage();

#endif
