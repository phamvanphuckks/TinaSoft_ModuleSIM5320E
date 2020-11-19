/**
 * Author: Tinasoft.vn
 */
#include "App.h"

/*
 * viet luon class trong app
*/

float getSOH(unsigned long t1, unsigned long t2, float u, float i)
{
  float ft2=0, ft1=0;
  ft2 = u*i*(t2*t2)/2;
  ft1 = u*i*(t1*t1)/2;

  return (ft2 - ft1);
}

/*
 * Depth of Discharge - Độ sâu phóng điện
 * DOD sẽ là lần xả gần nhất, bị ghi đè trong thẻ nhớ.
*/
unsigned long getDOD(unsigned long t1, unsigned long t2)
{
  return t2-t1;
}

/*  R1=1M1, R2 = 100k;
 *  DEFAULT : Đặt mức điện áp tối đa là 5V (nếu trên mạch dùng nguồn 5V làm nuôi chính) hoặc là 3,3V (nếu trên mạch dùng nguồn 3,3V làm nguồn nuôi chính).
 *  EXTERNAL : Đặt lại mức điện áp tối đa BẰNG với mức điện áp được cấp vào chân AREF ( Chỉ được cấp vào chân AREF một điện áp nằm trong khoảng 0-5V ).
*/
float getVoltageOfCircuit(void)
{
  float R1=1000000.0, R2=100000.0;
  float vol=0.0;
  /*
   * 0-1023 <-> 0-5V
  */ 
  vol = (analogRead(VOLTAGE)*5.0/1023.0)*(R1+R2)/R2;
  return vol;
}
