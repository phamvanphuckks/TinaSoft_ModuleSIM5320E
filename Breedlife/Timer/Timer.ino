#include<Arduino.h>
#include <avr/interrupt.h>

#define BUTTON1 6

/*
 * system clock frequency : 16Mhz
*/

void setup() {
  pinMode(BUTTON1, INPUT);
  Serial.begin(9600);
  cli(); // tắt ngắt toàn cục

  /*Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;

  /*Setup Timer/Counter1 */
  //TCCR1B |= (1<<CS11)|(1<<CS10); // prescale = 64
  TCCR1B |= (1<<CS12); // prescale = 256
  TCNT1 = 3034;
  TIMSK1 = (1 << TOIE1); // overflow interrupt enable
  sei(); // cho phép ngắt toàn cục
}

void loop() {
  // put your main code here, to run repeatedly:

}

ISR (TIMER1_OVF_vect) 
{
  TCNT1 = 3034;
  if(digitalRead(BUTTON1)==0){
  Serial.println("Temp:");
  }
}
