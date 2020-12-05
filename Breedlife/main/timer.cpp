#include "timer.h"

void BT_Timer_Init(void)
{
  /*Reset Timer/Counter1*/
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  
  /*Setup Timer/Counter1 */
  //TCCR1B |= (1 << CS11)|(1 << CS10); // prescale = 64
  TCCR1B |= (1 << CS12); // prescale = 256
  TCNT1 = 45412; // 300ms
  TIMSK1 = (1 << TOIE1); // overflow interrupt enable
  sei(); // cho phép ngắt toàn cục
  
}
void SIM_Timer_Init(void)
{
  /*Reset Timer/Counter5 */
  TCCR5A = 0;
  TCCR5B = 0;
  TIMSK5 = 0;
  
  /*Setup Timer/Counter5 */
  TCCR5B |= (1 << CS12)|(1 << CS10); // prescale = 1024
  TCNT5 = 49150;// 1s
  TIMSK5 = (1 << TOIE5); // overflow interrupt enable
  sei(); // cho phép ngắt toàn cục 
}

ISR (TIMER1_OVF_vect) 
{
  if((digitalRead(BT_UP) == 0) && (BT_UP_Flag == 0)){
    BT_UP_Flag = 1;
  }
   if((digitalRead(BT_ENTER) == 0) && (BT_ENTER_Flag == 0)){
    BT_ENTER_Flag = 1;
  }

  bool b6 = ((PINE >> PORTE6) & 0x01);
  //ECHOLN("b6:"+String(b6));
  if((b6 == 0) && (BT_DOWN_Flag == 0)){
    BT_DOWN_Flag = 1;
  }
  
  bool b7 = ((PINE >> PORTE7) & 0x01);
  //ECHOLN("b7:"+String(b7));
  if( (b7 == 0) && (BT_BACK_Flag == 0)){
    BT_BACK_Flag = 1;
  }

  TCNT1 = 45412; // 300ms
}

ISR (TIMER5_OVF_vect)
{
  SIM_Flag++;
  Sync_Flag++;
  TIM_SOH_Flag++;
  TIM_UpdateCurrent++;
  TCNT5 = 49150; // 1s
}
