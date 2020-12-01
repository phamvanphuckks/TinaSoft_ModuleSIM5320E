#include "ledbutton.h"

void Init_Buzz(void)
{
  pinMode(BUZZ, OUTPUT);
}

void Init_LED(void)
{
  pinMode(LED_RED,    OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN,  OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}
void RGB_Red()
{
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);  
}
void RGB_Blue()
{
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH); 
}
void RGB_Green()
{
   
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void Init_Button(void)
{
  pinMode(BT_UP, INPUT_PULLUP);
  pinMode(BT_ENTER, INPUT_PULLUP);

  // BT_DOWN, BT_BACK - input
  DDRE &= ~(1 << DDE7);
  DDRE &= ~(1 << DDE6);
  // pull-up
  PORTE |= B11000000;
}
