#ifndef __LEDBUTTON_H__
#define __LEDBUTTON_H__

#include <Arduino.h>
#include <avr/interrupt.h>
#include "AppDebug.h"

// chú ý dùng timer đọc nút nhấn
#define BT_UP     2
#define BT_DOWN   8
#define BT_BACK   9 
#define BT_ENTER  3

#define BUZZ 10

#define LED_RED     23
#define LED_YELLOW  24
#define LED_GREEN   25

#define LED_R       12
#define LED_G       11
#define LED_B       13

#define LEDON(x)        digitalWrite(x, HIGH);
#define LEDOOFF(x)      digitalWrite(x, LOW);

#define BUZZON       digitalWrite(BUZZ, HIGH);
#define BUZZOFF      digitalWrite(BUZZ, LOW);

extern volatile bool  BT_UP_Flag, BT_DOWN_Flag, BT_BACK_Flag, BT_ENTER_Flag; 

void Init_Buzz(void);
void Init_LED(void);
void Init_Button(void);
void RGB_Red(void);
void RGB_Blue(void);
void RGB_Green(void);

#endif
