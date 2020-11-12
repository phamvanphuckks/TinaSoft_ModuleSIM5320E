#include "ledbutton.h"

void Init_Buzz(void)
{
  pinMode(BUZZ, OUTPUT);
}

void Init_LED(void)
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void Init_Button(void)
{
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  /*Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  
  /*Setup Timer/Counter1 */
  //TCCR1B |= (1<<CS11)|(1<<CS10); // prescale = 64
  TCCR1B |= (1<<CS12); // prescale = 256
  TCNT1 = 34285;
  TIMSK1 = (1 << TOIE1); // overflow interrupt enable
  sei(); // cho phép ngắt toàn cục
}

ISR (TIMER1_OVF_vect) 
{
  TCNT1 = 34285;
  if(digitalRead(BUTTON1)==0){
    Serial.println("BUTTON1");
  }
  if(digitalRead(BUTTON2)==0){
    Serial.println("BUTTON2");
  }
  if(digitalRead(BUTTON3)==0){
    Serial.println("BUTTON3");
  }
   if(digitalRead(BUTTON4)==0){
    Serial.println("BUTTON4");
  }
}
