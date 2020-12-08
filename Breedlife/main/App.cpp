/**
 * Author: Tinasoft.vn
 */
#include "App.h"

// object
ACS712_Sensor ACS712(VIN, SEN, QV, MOE);

LDC1604Display LDC1604(0x20, 20, 4);

NTC10kSensor NTC10k;

DS1307Time DS1307;

SDCard  SDMemory;

SIM7600E  SIM;
//end-object

// variable, flag
uint8_t sohFlag = T0_Flag;
volatile uint16_t TIM_SOH_Flag = 0;
float Psum = 0.0;

volatile uint16_t  SIM_Flag = 0;
volatile uint16_t Sync_Flag = 0;
DataSIM *dataPackage = (DataSIM*)malloc(sizeof(DataSIM));

volatile bool BT_UP_Flag = 0, BT_DOWN_Flag = 0, BT_BACK_Flag = 0, BT_ENTER_Flag = 0; 
int pos_Menu = 0, pos_Rec = 0, pos_Batt = 0;
float LLVD_value = 0.0, offset_LLVD = 0.0;
uint8_t lcdFlag = HOME_Flag;

volatile uint16_t TIM_UpdateLCD = 0;
uint32_t INDEX_DATA = 0, INDEX_BACKUP = 0;
// end - variable, flag

/*
 * hàm khởi tạo các module
*/
void LCD1604_Init()
{
  // bật đèn nền
  pinMode(LCD_POWER, OUTPUT);
  digitalWrite(LCD_POWER, HIGH);
  delayMicroseconds(4500);
  LDC1604.init(); 
  delayMicroseconds(4500);                  
  LDC1604.setBacklight(true); 
  delayMicroseconds(4500);
  // Tạo mũi tên
  LDC1604.createChar(ARROW, iArrow);
  delayMicroseconds(4500);
  // Khởi tạo home
  LCD_home();
}

void DS1307_Init()
{
//  char s[5];
  DS1307.RTC_Init();
//  DS1307.RTC_getTime(s);
}

void SDCard_Init()
{
  SDMemory.SD_Init();
  // get pos
  INDEX_DATA   = EEPROM.readLong(addrPosDATA);
  INDEX_BACKUP = EEPROM.readLong(addrPosBACKUP);
}

void SIM7600E_Init()
{
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

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*
 * ACS712
*/
IsChargeStatus getIsCharging(void)
{
  return ACS712.isCharging();
}

// t(s)
unsigned long getTimeSOH(uint8_t t)
{
  switch(t)
  {
    case T0:
      return ((ACS712.t0_h*60 + ACS712.t0_m)*60 + ACS712.t0_s);
      break; 
    case T1:
      return ((ACS712.t1_h*60 + ACS712.t1_m)*60 + ACS712.t1_s); 
      break;
    case T1D:
      return ((ACS712.t1d_h*60 + ACS712.t1d_m)*60 + ACS712.t1d_s); 
      break;
    case T2:
      return ((ACS712.t2_h*60 + ACS712.t2_m)*60 + ACS712.t2_s);
      break;
    default:
      break;
  }
}

float getSOH(unsigned long t1, unsigned long t2)
{
  float Ft2 = 0.0, Ft1 = 0.0, F = 0.0;
  t2 = t2-t1;
  t1 = 1;

  Ft2 = ACS712.u_discharge*ACS712.i_discharge*(t2);
  Ft1 = ACS712.u_discharge*ACS712.i_discharge*(t1);
    
  ECHOLN("u_discharge: "+String(ACS712.u_discharge));
  ECHOLN("i_discharge: "+String(ACS712.i_discharge));
  ECHOLN("t1: "+String(t1));
  ECHOLN("t2: "+String(t2));
  ECHOLN("FT2: "+String(Ft2));
  ECHOLN("FT1: "+String(Ft1));
  
  F = (Ft2 - Ft1)/3600;
  ECHOLN(String(F)+": W/h");
  return (Ft2 - Ft1)/3600; // wh
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
  return (t2 - t1);
}

/*  R1=1M1, R2 = 100k;
 *  DEFAULT : Đặt mức điện áp tối đa là 5V (nếu trên mạch dùng nguồn 5V làm nuôi chính) hoặc là 3,3V (nếu trên mạch dùng nguồn 3,3V làm nguồn nuôi chính).
 *  EXTERNAL : Đặt lại mức điện áp tối đa BẰNG với mức điện áp được cấp vào chân AREF ( Chỉ được cấp vào chân AREF một điện áp nằm trong khoảng 0-5V ).
*/
float getVoltageOfCircuit(void)
{
  float R1 = 1000000.0, R2 = 100000.0;
  float vol = 0.0;
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


void Calulation_SOH_DOD(void)
{
  /* 
   * khi bộ sạc được cắm điện : bộ sạc vừa sạc cho accuy vừa cấp nguồn cho tải.
   * Khi mà accuy đầy : bộ sạc chỉ cung cấp cho tải <=> ACS712 đọc được 0A +- sai số 0.2 A, U = 49V.
   * (getVoltage() >=  53.0) : Đang cắm sạc  -- có 2 nguồn 49.0 V và 53 V <=> chênh áp lựa chọn 53.0V => getVoltage > 53.0V
   * (ACS712.getCurrent(ACS1) == 0.0) : mạch không sạc, không xả <=> đang cắm sạc, và bình accuy đầy.
  */  
  if((sohFlag == T0_Flag) && (getIsCharging() == NONE) && (getVoltageOfCircuit() >= 53.0)) 
  {
    ECHOLN("Phat hien mach khong sac, xa : t0");
    DS1307_getTime(T0);// t0 = timenow
    // ghi t0 vào thẻ nhớ
    
    sohFlag = T1_Flag;
  }

  /*
   * Nếu mà phát hiện ra đang sạc đầy và nhận thấy quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
   * (getVoltageOfCircuit() <= 49.0) : không cắm sạc, lúc này accuy xả cho tải. -- có 2 nguồn 49.0 V và 53.0 V <=> chênh áp lựa chọn 49.0V => getVoltage <= 49.0V
  */
  if((sohFlag == T1_Flag) && (getIsCharging() == DISCHARGE) && (getVoltageOfCircuit() <= 49.0))
  {
    ECHOLN("Phat hien mach xa : t1");
    DS1307_getTime(T1);   // t1 = timenow
    DS1307_getTime(T1D); // t1d = timenow;
    Psum = 0.0;
    // ghi t1 vào SD card
    
    // thiếu đọc liên tục các giá trị dòng điện và điện áp vào SD card

    // lấy u xả và i xả
    getUIdischarge();

    sohFlag = T2_Flag|SOH_Flag;
  }
  /*
   * Nếu đang xả của quá trình đo thời gian xả đế tính SOH và DOD -> căm sạc vào , hủy bỏ lần tính toán đấy.
   * Có 2 nguồn 49.0 V và 53.0 V <=> chênh áp lựa chọn 53.0V => getVoltage <= 53.0V
   * => không tính lần SOH, DOD đấy.
  */
  if((sohFlag & T2_Flag) && (getIsCharging() == CHARGE) && (getVoltageOfCircuit() >= 53.0))
  {
    ECHOLN("Phat hien mach xa, bi cam sac");
    Psum = 0.0;

    // xóa cờ phục vụ chu kì mới
    sohFlag = T0_Flag;
  }
  
  if((sohFlag & SOH_Flag) && (getIsCharging() == DISCHARGE) && (TIM_SOH_Flag == 5)) // 5s 
  {
    unsigned long t1 = 0, t2 = 0;
    
    getUIdischarge();
    DS1307_getTime(T2);// t2 = timenow
    t1 = getTimeSOH(T1);
    t2 = getTimeSOH(T2);

    Psum += getSOH(t1, t2);
    DS1307_getTime(T1);
    TIM_SOH_Flag = 0;
  }
  /*
   * I = 0, U <= 40 (Vẫn nối với tải - tải với nguồn cấp cho mạch chung) => bình cạn I
   * Tính SOH và DOD
  */
  if((sohFlag & SOH_Flag) && (getIsCharging() == NONE) && (getVoltageOfCircuit() <= 40.0))// bình cạn
  {
    ECHOLN("Phat hien mach xa het, tinh toan SOH, DOD: t2");
    unsigned long t1 = 0, t1d = 0,  t2 = 0;
    float SOH = 0.0, DOD = 0.0;
    getUIdischarge();
    DS1307_getTime(T2);// t2=timenow
    t1  = getTimeSOH(T1);
    t1d = getTimeSOH(T1D);
    t2  = getTimeSOH(T2);

    // tính toán SOH,DOC
    SOH = Psum + getSOH(t1, t2);
    DOD = getDOD(t1d, t2);
    
    // lưu vào thẻ nhớ
    SDCard_saveSOH_DOD("SOH_DOD.txt", SOH, DOD);    

    // xóa cờ, thực hiện chu kì tính toán SOH, DOD mới.
    sohFlag = T0_Flag;
  }

  /*
   * I=0, U>40(tải rút ra - tải, nguồn cấp cho mạch chung) bình ko cạn, nhưng bị rút tải
   * không tính gì cả, chỉ lưu lại giá trị t2 tính DOD
  */
  else if((sohFlag & T2_Flag) && (getIsCharging() == NONE) && (getVoltageOfCircuit() > 40.0))
  {
    ECHOLN("Phat hien mach xa het, nhung bi rut tai");
    unsigned long t1d = 0, t2 = 0;
   
    DS1307_getTime(T2);// t2=timenow
    t1d = getTimeSOH(T1D);
    t2 = getTimeSOH(T2);

    // tính toán DOD
    getDOD(t1d, t2);

    // xóa cờ thực hiện chu kì mới
    sohFlag = T0_Flag;
  }  
}
/*end-acs712*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*
 * LCD1604
 * column, row : 0->n-1.
*/
void LCD_home()
{
  float voltage = 0.0, iload = 0.0, ibattery = 0.0;
  char t_data[30];

  voltage = getVoltageOfCircuit();
  
  if(getIsCharging() == CHARGE)
  {
    iload = 0;
    ibattery = ACS712.getAmpleOfCircuit();
  }
  else if(getIsCharging() == DISCHARGE)
  {
    iload = ACS712.getAmpleOfCircuit();
    ibattery = 0;   
  }
  else
  {
    iload = 0;
    ibattery = 0;
  }
  
  LDC1604.clear();
  LDC1604.setCursor(5, 0);
  DS1307.RTC_getDate(t_data);
  LDC1604.print(t_data);
  LDC1604.setCursor(0, 1);
  LDC1604.print("V:" + String(voltage));
  LDC1604.setCursor(10, 1);
  LDC1604.print("Tempe:" + String(NTC10k.NTC10k_ReadTemperture(NTC10kpin1)));
  LDC1604.setCursor(18, 1);
  LDC1604.print("*C");
  LDC1604.setCursor(0, 2);
  LDC1604.print("Load:" + String(iload));
  LDC1604.setCursor(0, 3);
  LDC1604.print("Battery:" + String(ibattery));
}

void LCD_LLVD(float offset)
{
  LLVD_value = EEPROM.readFloat(addrLLVD);
  LLVD_value += offset;
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("LLVD");
  LDC1604.setCursor(7, 1);
  LDC1604.print(LLVD_value);
  LDC1604.print("V");
}

void LCD_battery(uint8_t indexBat)
{
  float voltage = 0.0 , i = 0.0, imax = 0.0;
  voltage = getVoltageOfCircuit();
  i       = ACS712.getCurrent((indexBat + 55));
  imax    = 20;
  char stt[] = "Normal";
  
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("Battery" + String(indexBat));
  LDC1604.setCursor(14, 1);
  LDC1604.print("I:" + String(i) + "A");
  LDC1604.setCursor(0, 1);
  LDC1604.print("U:" + String(voltage) + "V");
  LDC1604.setCursor(0, 2);
  LDC1604.print("Imax:" + String(imax) + "A");
  LDC1604.setCursor(0, 3);
  LDC1604.print("Status:" + String(stt));
}

void LCD_rectifiers(uint8_t indexRec, uint8_t voltage, uint8_t i, uint8_t imax, char *stt)
{
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("Rectifiers"+String(indexRec));
  LDC1604.setCursor(15, 1);
  LDC1604.print("I:" + String(i) + "A");
  LDC1604.setCursor(0, 1);
  LDC1604.print("U:" + String(voltage) + "V");
  LDC1604.setCursor(0, 2);
  LDC1604.print("Imax:" + String(imax) + "A");
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
  LDC1604.print("LLVD");
}

void LCD_arrow(uint8_t colum, uint8_t row)
{
  for(int i = 1; i <= 3 ;i++)
  {
    if(i == row)
    {
      LDC1604.setCursor(colum, row);
      LDC1604.write(ARROW); 
      continue;
    }
    LDC1604.setCursor(0, i);
    LDC1604.write(' '); 
  } 
}

void move_up()
{
  if(BT_UP_Flag == SET)
  {
    if(lcdFlag == REC_Flag) //up rectifiers
    {
      ++pos_Rec;
      if(pos_Rec > 9) pos_Rec = 0;
      LCD_rectifiers(pos_Rec + 1, 8, 10, 30, "Normal");
    }
    else if(lcdFlag == BAT_Flag) //up battery
    {
      ++pos_Batt;
      if(pos_Batt > 9)  pos_Batt = 0;
      LCD_battery(pos_Batt + 1);
    }
    else if(lcdFlag == LLVD_Flag)//up LLVD
    {
      offset_LLVD += 0.1;
      LCD_LLVD(offset_LLVD);
    }
    else if(lcdFlag & MENU_Flag) // up menu
    {
      ++pos_Menu;
      if(pos_Menu > 2)  pos_Menu = 0;
      switch(pos_Menu)
      {
        case 0:
          lcdFlag = (REC_Flag|MENU_Flag);
          LCD_arrow(0, 1);
          break;
        case 1:
          lcdFlag = (BAT_Flag|MENU_Flag);        
          LCD_arrow(0, 2);
          break;
        case 2:
          lcdFlag = (LLVD_Flag|MENU_Flag);           
          LCD_arrow(0, 3);     
          break;
        default:
          break;
      }
    }
    BT_UP_Flag = CLEAR;
  }
  else
  {
    BT_UP_Flag = CLEAR;
  }
}

void move_down(void)
{
  if(BT_DOWN_Flag == SET)
  {
    if(lcdFlag == REC_Flag) // down rectifiers
    {
      --pos_Rec;
      if(pos_Rec < 0) pos_Rec = 9;
      LCD_rectifiers(pos_Rec+1, 10, 20, 30, "Normal");
    }
    else if(lcdFlag == BAT_Flag)// down batteries
    {
      --pos_Batt;
      if(pos_Batt < 0)  pos_Batt = 9;
      LCD_battery(pos_Batt+1);
    }
    else if(lcdFlag == LLVD_Flag) // down LLDV
    {
      offset_LLVD -= 0.1;
      LCD_LLVD(offset_LLVD);
    }
    else if(lcdFlag & MENU_Flag) // down menu
    {
      --pos_Menu;
      if(pos_Menu < 0) pos_Menu = 2;
      switch(pos_Menu)
      {
        case 0:
          lcdFlag = (MENU_Flag|REC_Flag);
          LCD_arrow(0, 1);
          break;
        case 1:
          lcdFlag = (MENU_Flag|BAT_Flag);
          LCD_arrow(0, 2);
          break;
        case 2:
          lcdFlag = (MENU_Flag|LLVD_Flag);
          LCD_arrow(0, 3);
          break;
        default:
          break;
      }
    }
    BT_DOWN_Flag = CLEAR;
  }
  else
  {
    BT_DOWN_Flag = CLEAR;
  }
}

void move_back(void)
{
  if(BT_BACK_Flag == SET) 
  {
    if((lcdFlag == REC_Flag) || (lcdFlag == BAT_Flag) || (lcdFlag == LLVD_Flag))
    {
      // back  từ 2 - 1,  từ rectifier, battery, LLVD quay lại menu
      lcdFlag = (MENU_Flag|REC_Flag); 
      LCD_menu();
      LCD_arrow(0, 1);     
      
      // đặt lại chỉ mục 
      pos_Rec  = 0; 
      pos_Batt = 0;
      pos_Menu = 0;
      
      offset_LLVD = 0;
    }
    else if(lcdFlag & MENU_Flag) // từ menu quay về home
    {
      lcdFlag = HOME_Flag; 
      // đặt lại chỉ mục 
      pos_Rec  = 0; 
      pos_Batt = 0;
      pos_Menu = 0;

      // hiển thị home
      LCD_home();
    }    
    BT_BACK_Flag = CLEAR;
  }
  else
  {
    BT_BACK_Flag = CLEAR;
  }  
}

void move_enter(void)
{
  if(BT_ENTER_Flag == SET)
  {
    if(lcdFlag & MENU_Flag) // nếu đang ở Menu -> press enter/menu, vào 1 trong 3 submenu
    {
      if(lcdFlag & REC_Flag) // rectifiers
      {
        lcdFlag = REC_Flag;
        LCD_rectifiers(pos_Rec+1, 10, 20, 30, "Normal");
      }
      else if(lcdFlag & BAT_Flag) // batteries 
      {
        lcdFlag = BAT_Flag;
        LCD_battery(pos_Batt+1);  
      }
      else if(lcdFlag & LLVD_Flag) // LLVD
      {
        lcdFlag = LLVD_Flag;
        LCD_LLVD(offset_LLVD);
      }
    }
    else if(lcdFlag == LLVD_Flag) // LLVD
    {
      lcdFlag = LLVD_Flag;
      if(offset_LLVD != 0) // submenu LLVD:  ấn enter/menu - save LLVD
      {
        EEPROM.writeFloat(addrLLVD, (EEPROM.readFloat(addrLLVD)+offset_LLVD));
        offset_LLVD = 0;
        LDC1604.setCursor(9, 2); 
        LDC1604.print("OK");
        long t=4800000;
        while(t--)
        {
          __asm__("nop\n\t");
        } 
        LDC1604.setCursor(9, 2); 
        LDC1604.print("  ");
      }
      LCD_LLVD(offset_LLVD);
    }
    else if(lcdFlag == HOME_Flag)// vị trí home - nhấn enter/menu để vào menu
    {
      LCD_menu();
      LCD_arrow(0, 1);
      lcdFlag = (MENU_Flag|REC_Flag); // vào menu và ấn menu lần nữa -> submenu: rectifiers
    }
    BT_ENTER_Flag = CLEAR;
  }
  else
  {
    BT_ENTER_Flag = CLEAR;
  } 
}
/*end -lcd1604*/


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
      DS1307.RTC_getTime(&ACS712.t1_h, &ACS712.t1_m, &ACS712.t1_s); // t1=timenow
      break;
     case T1D:
      DS1307.RTC_getTime(&ACS712.t1d_h, &ACS712.t1d_m, &ACS712.t1d_s); // t1=timenow
      break;
     case T2:
      DS1307.RTC_getTime(&ACS712.t2_h, &ACS712.t2_m, &ACS712.t2_s); // t2=timenow
      break;
     default:
      break;
  }
}

/*
 * SD-Card
*/
// Lưu giá trị SOH, DOD vào thẻ nhớ
bool SDCard_saveSOH_DOD(char *namefile, float SOH, float DOD)
{
  SDMemory.SD_openFile(namefile, FILE_WRITEF); 

  // through the header fied
  while (SDMemory.SD_available()) 
  {   
    char inputChar = SDMemory.SD_readFile(); // Gets one byte from serial buffer
    if (inputChar == '\n') //end of line (or 10)
    {
      break;
    }
  }
  // the data fied
  SDMemory.SD_print(SOH);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_println(DOD);
  SDMemory.SD_closeFile(namefile);
}
// lấy giá trị SOH, DOD
bool SDCard_getSOH_DOD(char *namefile, float *SOH, float *DOD)
{
  SDMemory.SD_openFile(namefile, FILE_READ);
  SDMemory.SD_readFile(SOH);
  SDMemory.SD_readFile(DOD);
  SDMemory.SD_closeFile(namefile);
}
// lưu thông tin vào thẻ nhớ
bool SDCard_saveInfo(char *namefile, DataSIM *data)
{
  //SDMemory.SD_writeFile(namefile, "STT,Time,U,I,I1,I2,I3,I4,I5,I6,I7,I8,I9,I10,P,SOH,DOD\n");
  SDMemory.SD_openFile(namefile, FILE_WRITE);

  SDMemory.SD_print(INDEX_DATA++);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_writeFile(namefile, data->t);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_print(data->U);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_print(data->I);
  SDMemory.SD_writeFile(namefile, ",");
  
  for(int idex = 56; idex < 66; idex++)
  {
    SDMemory.SD_print(data->acs[idex-56]);
    SDMemory.SD_writeFile(namefile, ",");
  }
  
  SDMemory.SD_print(data->P);
  SDMemory.SD_writeFile(namefile, ",");

  SDMemory.SD_print(data->SOH);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_println(data->DOD);
  
  SDMemory.SD_closeFile(namefile);
}

//// lấy dữ liệu backup
//bool SDCard_getDataBackUp(char *namefile, DataSIM *data)
//{
//  SDMemory.SD_openFile(namefile, FILE_WRITE); 
//  
//  SDMemory.SD_closeFile(namefile);
//}
//
//// xóa cột data thứ nhất
//bool SDCard_setOk(char *namefile, int row, column)
//{
//  int posRow=0;
//  SDMemory.SD_openFile(namefile, FILE_WRITEF);
//
//  while (SDMemory.SD_available() && posRow < row+2) 
//  {   
//   char inputChar = SDMemory.SD_readFile(); // Gets one byte from serial buffer
//    if (inputChar == '\n') //end of line (or 10)
//    {
//      posRow++;
//    }
//  }  
//
//  return true;
//}

/*
 * ntc10k 
*/
float getTemperture(uint8_t ntc)
{
  return NTC10k.NTC10k_ReadTemperture(ntc);
}

/*
 * SIM7600E
*/

static void getData(DataSIM *data)
{
  strcpy(data->url, "http://admin.breedlife.com/api/v1/data");
  DS1307.RTC_getDateTime(data->t);

  for(int i = 0; i<10;i++)
  {
    data->acs[i] = ACS712.getCurrent(i+56);
  }
  data->U = getVoltageOfCircuit();
  data->I = ACS712.getAmpleOfCircuit();
  data->P = (data->U)*(data->I);
  SDCard_getSOH_DOD("SOH_DOD.txt", &data->SOH, &data->DOD);

  // về sau phải tính toán gói tin này.
  strcpy(data->data, "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}");
}

bool SIM_POST_Data(char *url, char *data)
{
  return SIM.HTTP_POST_start(url, data);
}

bool SIM_UpDate(uint16_t times)
{
  if(SIM_Flag >= times)
  {
    //getData(dataPackage);
    strcpy(dataPackage->url, "http://admin.breedlife.com/api/v1/data");
    strcpy(dataPackage->data, "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}");
    if(!SIM_POST_Data(dataPackage->url, dataPackage->data)) // gui du lieu len server
    {
        SDCard_saveInfo("backup.csv", dataPackage); 
        SIM_Flag = 0;
        return false;
    }
    ECHOLN("\tpush data to server");
    SDCard_saveInfo("data.csv", dataPackage); // lưu vào thẻ nhớ
    SIM_Flag = 0;
    return true;
  }
}

bool SIM_Synch(uint16_t times)
{
  if(Sync_Flag == times)
  {
    //getData(dataPackage);
    if(!SIM_POST_Data(dataPackage->url, dataPackage->data)) // đồng bộ không thành công 
    {
      return false;
    }  
    //SDCard_clearRow("backup.csv"); // đồng bộ - xóa hàng vừa đồng bộ
    Sync_Flag = 0;
    return true;
  }
}

/* 
 *  debug
 */
void ACS712_Debug(int ACSx)
{
  ACS712.debug(ACSx); 
}

/*
 * back up - khi mất điện
 */
void back_up(void)
{
  // lưu lại vị trí STT trong file data.csv và backup.csv
  EEPROM.writeLong(addrPosDATA, INDEX_DATA);
  EEPROM.writeLong(addrPosBACKUP, INDEX_BACKUP);
}
