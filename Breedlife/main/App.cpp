/**
 * Author: Tinasoft.vn
 */
#include "App.h"

ACS712_Sensor ACS712(VIN, SEN, QV, MOE);

//note  old version 0X27, new version 0x3F or 0x20
LDC1604Display LDC1604(0x20, 20, 4);

DS1307Time DS1307;

NTC10kSensor NTC10k;

SDCard  SDMemory;

SIM7600E  SIM;

void LCD1604_Init()
{
  LDC1604.init();                    
  LDC1604.setBacklight(true); 

  // bật đèn nền
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  //  tạo mũi tên
  LDC1604.createChar(ARROW, iArrow);
  // Khởi tạo home
  LCD_home();
}
void DS1307_Init()
{
  DS1307.RTC_Init();
}
void SDCard_Init()
{
  SDMemory.SD_Init();
}
void SIM7600E_Init()
{
  delay(5000);
  if(SIM.setupSIM(9600))
  { 
    ECHOLN(F("\t----------------------------------"));
    ECHOLN(F("\tSettup done"));
    ECHOLN(F("\t----------------------------------"));
  }
  else
  {
    ECHOLN(F("\t----------------------------------"));
    ECHOLN("\tSettup fail");
    ECHOLN(F("\t----------------------------------"));
  }

  SIM_Timer_Init();
}
void Led_Button_Buzz_Init()
{
  Init_Buzz();
  Init_LED();
  Init_Button();
  BT_Timer_Init();    
}

/*
 * ACS712
*/

IsChargeStatus getIsCharging(void)
{
  return ACS712.isCharging();
}

unsigned long getTimeSOH(uint8_t t)
{
  switch(t)
  {
    case T1:
      return ACS712.t1_h*60*ACS712.t1_m*60 + ACS712.t1_s;
      break;
    case T2:
      return ACS712.t2_h*60*ACS712.t2_m*60 + ACS712.t2_s;
      break;
  }
}

float getSOH(unsigned long t1, unsigned long t2)
{
  float ft2 = 0, ft1 = 0;
  ft2 = ACS712.u_discharge*ACS712.i_discharge*(t2*t2)/2;
  ft1 = ACS712.u_discharge*ACS712.i_discharge*(t1*t1)/2;

  return (ft2 - ft1);
}
/*
 * Tính toán % soh -> lưu vào thẻ nhớ, hiển thị đèn
*/
float getPercentSOH(unsigned long t1, unsigned long t2)
{


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

void getUIdischarge(void)
{
  ACS712.u_discharge = getVoltageOfCircuit();
  ACS712.i_discharge = ACS712.getAmpleOfCircuit();
}

/*
 * LCD1604
*/

void LCD_displayACS(int ACSx)
{
 LDC1604.clear();
 switch (ACSx)
 {
  case ACS1:
    break;
  case ACS2:
    break;
  case ACS3:
    break;
  case ACS4:
    break;
  case ACS5:
    break;
  case ACS6:
    break;
   case ACS7:
    break;
  case ACS8:
    break;
   case ACS9:
    break;
  case ACS10:
    break;
  default:
    break;   
 }
}

// column, row : 0->n-1.
void LCD_home(void)
{
  char t_data[20];
  
  LDC1604.clear();
  LDC1604.setCursor(5, 0);
  DS1307.RTC_getDate(t_data);
  ECHOLN(t_data);
  LDC1604.print(t_data);
  LDC1604.setCursor(0, 1);
  LDC1604.print("Vsum:");
  //LDC1604.print();
  LDC1604.setCursor(10, 1);
  LDC1604.print("Tempe:");
  //LDC1604.print();
  LDC1604.setCursor(18, 1);
  LDC1604.print("*C");
  
  LDC1604.setCursor(0, 2);
  LDC1604.print("Load: ");
  //LDC1604.print();
  LDC1604.setCursor(0, 3);
  LDC1604.print("Battery: ");
  //LDC1604.print();
}

void LCD_LLDV()
{
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("LLDV");
  LDC1604.setCursor(8, 0);
  //LDC1604.print(LLDV);
}

void LCD_battery(uint8_t indexBat, uint8_t voltage, uint8_t i, uint8_t imax, char *stt)
{
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("Battery"+String(indexBat));
  LDC1604.setCursor(15, 1);
  LDC1604.print("I:"+String(i)+"A");
  LDC1604.setCursor(0, 1);
  LDC1604.print("U:" + String(voltage)+"V");
  LDC1604.setCursor(0, 2);
  LDC1604.print("Imax:" + String(imax)+"A");
  LDC1604.setCursor(0, 3);
  LDC1604.print("Status:" + String(stt));
}

void LCD_rectifiers(uint8_t indexRec, uint8_t voltage, uint8_t i, uint8_t imax, char *stt)
{
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("Rectifiers"+String(indexRec));
  LDC1604.setCursor(15, 1);
  LDC1604.print("I:"+String(i)+"A");
  LDC1604.setCursor(0, 1);
  LDC1604.print("U:" + String(voltage)+"V");
  LDC1604.setCursor(0, 2);
  LDC1604.print("Imax:" + String(imax)+"A");
  LDC1604.setCursor(0, 3);
  LDC1604.print("Status:" + String(stt));
}

void LCD_menu()
{
  LDC1604.clear(); 
  LDC1604.setCursor(0, 0); 
  LDC1604.print("Menu"); 
  LDC1604.setCursor(2, 1); 
  LDC1604.print("Rectifiers");
  LDC1604.setCursor(2, 2); 
  LDC1604.print("Battery");
  LDC1604.setCursor(2, 3); 
  LDC1604.print("LLDV");
}

void LCD_arrow(uint8_t colum, uint8_t row)
{
  LDC1604.setCursor(colum, row);
  LDC1604.write(ARROW);  
}

/*
 * DS1307
 */
void DS1307_getTime(uint8_t t)
{
  switch(t)
  {
    case T0:
      DS1307.RTC_getTime(&ACS712.t0_h, &ACS712.t0_m, &ACS712.t0_s); // t0=timenow
      break;
     case T1:
      DS1307.RTC_getTime(&ACS712.t1_h, &ACS712.t1_m, &ACS712.t1_s); // t0=timenow
      break;
     case T2:
      DS1307.RTC_getTime(&ACS712.t2_h, &ACS712.t2_m, &ACS712.t2_s); // t0=timenow
      break;
     default:
      break;
  }
}

/*
 * SD-Card
*/


/*
 * SIM7600E
*/

bool SIM_POST_Data(char *url, char *data)
{
  SIM.HTTP_POST_start(url, data);
}
/* 
 *  debug
 */
void ACS712_Debug(void)
{
  ACS712.debug(ACS1); 
}
