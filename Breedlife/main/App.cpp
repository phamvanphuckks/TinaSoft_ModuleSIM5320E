/**
 * Author: Tinasoft.vn
 */
#include "App.h"

ACS712_Sensor ACS712(VIN, SEN, QV, MOE);

LDC1604Display LDC1604(0x20, 20, 4);

NTC10kSensor NTC10k;

DS1307Time DS1307;

SDCard  SDMemory;

SIM7600E  SIM;

/*
 * hàm khởi tạo các module
*/
void LCD1604_Init()
{
  LDC1604.init();                    
  LDC1604.setBacklight(true); 

  // bật đèn nền
  pinMode(LCD_POWER, OUTPUT);
  digitalWrite(LCD_POWER, HIGH);
  // Tạo mũi tên
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
  // get pos
  INDEX_DATA   = EEPROM.readLong(addrPosDATA);
  delay(5);
  INDEX_BACKUP = EEPROM.readLong(addrPosBACKUP);
  delay(5);
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

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
      return (ACS712.t1_h*60 + ACS712.t1_m)*60 + ACS712.t1_s; // s
      break;
    case T2:
      return (ACS712.t2_h*60 + ACS712.t2_m)*60 + ACS712.t2_s; // s
      break;
  }
}

float getSOH(unsigned long t1, unsigned long t2)
{
  float Ft2 = 0, Ft1 = 0;
  Ft2 = ACS712.u_discharge*ACS712.i_discharge*(t2*t2)/2;
  Ft1 = ACS712.u_discharge*ACS712.i_discharge*(t1*t1)/2;

  return (Ft2 - Ft1);
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
   * Khi mà accuy đầy :  bộ sạc chỉ cung cấp cho tải <=> ACS712 đọc được 0A +- sai số 0.2A, U=48V.
   * (getVoltage() >= 40.0) : Loại bỏ trường hợp bị mất điện.
   * (ACS712.getCurrent(ACS1) == 0.0) : mạch không sạc, không xả <=> đang cắm sạc, và bình accuy đầy.
   * (t1_Flag == 0) : Không lặp lại ghi t0 vào thẻ nhớ khi accuy đầy
  */  

  if((t0_Flag == CLEAR) && (getIsCharging() == NONE) && (getVoltageOfCircuit() >= 40.0)) 
  {
    ECHOLN("Phat hien mach khong sac, xa : t0");
    DS1307_getTime(T0);// t0=timenow
    // ghi t0 vào thẻ nhớ
    
    t0_Flag = SET;
    t1_Flag = SET;
    t2_Flag = CLEAR;
  }

  /*
   * Nếu mà phát hiện ra đang sạc đầy và nhận thấy quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
   * t1_Flag == 0) : Không lặp lại quá trình ghi T2
   * (getVoltageOfCircuit() < 40.0) : không cắm sạc,lúc này accuy xả cho tải.
  */
  if((t1_Flag == SET) && (getIsCharging() == DISCHARGE) && (getVoltageOfCircuit() < 40.0))
  {
    ECHOLN("Phat hien mach xa : t1");
    DS1307_getTime(T1);// t1=timenow
    // ghi t1 vào SD card
    
    // thiếu đọc liên tục cá giá trị dòng điện và điện áp vào SD card

    // lấy u xả và i xả
    getUIdischarge();

    // set cờ tính toán SOH
    t0_Flag = SET;
    t1_Flag = CLEAR;
    t2_Flag = SET;
  }

  /*
   * Nếu đang xả của quá trình đo thời gian xả đế tính SOH và DOD -> căm sạc vào , hủy bỏ lần tính toán đấy.
   * => không tính lần SOH, DOD đấy.
  */
  if((t2_Flag == SET) && (getIsCharging() == CHARGE) && (getVoltageOfCircuit() >= 40.0))
  {
    ECHOLN("Phat hien mach xa, bi cam xac");

    // xóa cờ phục vụ chu kì mới
    t0_Flag = CLEAR;
    t1_Flag = CLEAR;
    t2_Flag = CLEAR;
  }

  /*
   * I=0, U<40 => bình cạn
   * Tính SOH và DOD
  */
  if( (t2_Flag == SET) && (getIsCharging() == NONE) && (getVoltageOfCircuit() < 40))// bình cạn
  {
    ECHOLN("Phat hien mach xa het, tinh toan SOH, DOD: t2");
    unsigned long t1 = 0, t2 = 0;
    float SOH = 0, DOD = 0;
   
    DS1307_getTime(T2);// t2=timenow
    t1 = getTimeSOH(T1);
    t2 = getTimeSOH(T2);

    // tính toán SOH,DOC
    SOH = getSOH(t1, t2);
    DOD = getDOD(t1, t2);
    
    // lưu vào thẻ nhớ
    SDCard_saveSOH_DOD("SOH_DOD.txt",SOH, DOD);    

    // xóa cờ, thực hiện chu kì tính toán SOH, DOD mới.
    t0_Flag = CLEAR;
    t1_Flag = CLEAR;
    t2_Flag = CLEAR;
  }

  /*
   * I=0, U>=40 bình ko cạn, nhưng bị rút tải
   * không tính gì cả, chỉ lưu lại giá trị t2 tính DOD
  */
  else if( (t2_Flag == SET) && (getIsCharging() == NONE) && (getVoltageOfCircuit() >= 40.0))
  {
    ECHOLN("Phat hien mach xa het, nhung bi rut tai");
    unsigned long t1=0, t2=0;
   
    DS1307_getTime(T2);// t2=timenow
    t1 = getTimeSOH(T1);
    t2 = getTimeSOH(T2);

    // tính toán DOD
    getDOD(t1, t2);

    // xóa cờ thực hiện chu kì mới
    t0_Flag = CLEAR;
    t1_Flag = CLEAR;
    t2_Flag = CLEAR;
  }  
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * LCD1604
 * column, row : 0->n-1.
*/
void LCD_home(void)
{
  float voltage = 0 , iload = 0, ibattery = 0;
  char t_data[20];

  voltage = getVoltageOfCircuit();
  
  if(getIsCharging() == CHARGE)
  {
    iload = 0;
    ibattery = ACS712.getAmpleOfCircuit();
  }
  else if(getIsCharging()==DISCHARGE)
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
  LDC1604.print("V:");
  LDC1604.print(voltage);
  LDC1604.setCursor(10, 1);
  LDC1604.print("Tempe:");
  LDC1604.print(NTC10k.NTC10k_ReadTemperture(NTC10kpin1));
  LDC1604.setCursor(18, 1);
  LDC1604.print("*C");
  
  LDC1604.setCursor(0, 2);
  LDC1604.print("Load: ");
  LDC1604.print(iload);
  LDC1604.setCursor(0, 3);
  LDC1604.print("Battery: ");
  LDC1604.print(ibattery);
}

void LCD_LLVD(float offset)
{
  LLVD_value = EEPROM.readFloat(addrLLVD) + offset;
  LDC1604.clear();  
  LDC1604.setCursor(0, 0);
  LDC1604.print("LLDV");
  LDC1604.setCursor(8, 0);
  LDC1604.print(LLVD_value);
}

void LCD_battery(uint8_t indexBat)
{
  // ACS712
  float voltage = 0 , i = 0, imax = 0;
  voltage = getVoltageOfCircuit();
  i = ACS712.getCurrent((indexBat+55));
  imax = 20;
  char stt[] = "Normal";
  
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
  LDC1604.print("LLVD");
}

void LCD_arrow(uint8_t colum, uint8_t row)
{
  LDC1604.setCursor(colum, row);
  LDC1604.write(ARROW);  
}

void move_up()
{
  if((BT_UP_Flag == 1) && (lcd_Menu_Flag == 1)) // up menu
  {
    ++pos_Menu;
    if(pos_Menu > 2)
    { 
      pos_Menu = 0;
    }
    ECHOLN("pos_Menu:"+String(pos_Menu));
    switch(pos_Menu)
    {
      case 0:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 1;
        lcd_Batt_Flag = 0;
        lcd_LLVD_Flag = 0;

        // mũi tên
        LCD_arrow(0, 1);
        break;
      case 1:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 1;
        lcd_LLVD_Flag = 0;

        // mũi tên
        LCD_arrow(0, 2);
        break;
      case 2:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 0;
        lcd_LLVD_Flag = 1; 
        
        // mũi tên
        LCD_arrow(0, 3);     
        break;
      default:
        break;
    }
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1) && (lcd_Rec_Flag == 1) && (lcd_Menu_Flag == 0)) // up rectifiers
  {
    ++pos_Rec;
    
    if(pos_Rec > 9)
    {
      pos_Rec = 0;
    }
    ECHOLN("pos_Rec:"+String(pos_Rec));
    LCD_rectifiers(pos_Rec+1, 8, 10, 30,"Normal");
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1) && (lcd_Batt_Flag == 1) && (lcd_Menu_Flag == 0)) // up battery
  {
    ++pos_Batt;
    if(pos_Batt > 9)
    {
      pos_Batt = 0;
    }
    ECHOLN("pos_Batt:"+String(pos_Batt));
    LCD_battery(pos_Batt+1);
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1) && (lcd_LLVD_Flag == 1) &&  (lcd_Menu_Flag == 0))
  {
    offset_LLVD += 0.1;
    LCD_LLVD(offset_LLVD);
  }
  else if((BT_UP_Flag == 1))
  {
    BT_UP_Flag = 0;
  }  
}

void move_down(void)
{
  // down
  if((BT_DOWN_Flag == 1) && (lcd_Menu_Flag == 1)) //  down menu
  {
    --pos_Menu;
    if(pos_Menu < 0)
    {
      pos_Menu = 2;
    }
    ECHOLN("pos_Menu:"+String(pos_Menu));
    switch(pos_Menu)
    {
      case 0:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 1;
        lcd_Batt_Flag = 0;
        lcd_LLVD_Flag = 0;

        // mũi tên
        LCD_arrow(0, 1);
        break;
      case 1:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 1;
        lcd_LLVD_Flag = 0;

        // mũi tên
        LCD_arrow(0, 2);
        break;
      case 2:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 0;
        lcd_LLVD_Flag = 1;

        // mũi tên
        LCD_arrow(0, 3);
        break;
      default:
        break;
    }
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1)  && (lcd_Rec_Flag == 1) && ((lcd_Menu_Flag == 0))) // down rectifiers
  {
    --pos_Rec;
    if(pos_Rec < 0)
    {
      pos_Rec = 9;
    }
    ECHOLN("pos_Rec:"+String(pos_Rec));
    LCD_rectifiers(pos_Rec+1, 10, 20, 30, "Normal");
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1)  && (lcd_Batt_Flag == 1) && (lcd_Menu_Flag == 0))// down batteries
  {
    --pos_Batt;
    if(pos_Batt < 0)
    {
      pos_Batt = 9;
    }
    
    ECHOLN("pos_Batt:"+String(pos_Batt));
    LCD_battery(pos_Batt+1);
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1) && (BT_DOWN_Flag == 1) &&  (BT_DOWN_Flag == 0))
  {
    offset_LLVD -= 0.1;
    LCD_LLVD(offset_LLVD);
  }
  else if((BT_DOWN_Flag == 1))
  {
    BT_DOWN_Flag = 0;
  }  
}

void move_back(void)
{
  // back
  if((BT_BACK_Flag == 1) && ((pos_PC != 0)))
  {
    if(pos_PC == 2) // back  từ 2 - 1 - từ rectifier, battery, LLDV quay lại menu
    {
      // đặt lại cờ
      lcd_Home_Flag = 0;
      lcd_Menu_Flag = 1;
      lcd_Rec_Flag  = 1;
      lcd_Batt_Flag = 0;
      lcd_LLVD_Flag = 0;

      // hiển thị menu + mũi tên
      LCD_menu();
      LCD_arrow(0, 1);     

      // đặt lại chỉ mục 
      pos_Rec  = 0; 
      pos_Batt = 0;
      pos_Menu = 0;

      // đưa PC về 1
      pos_PC = 1;
      offset_LLVD = 0;
    }
    else if(pos_PC == 1) // từ menu quay về home
    {
      // đặt lại cờ
      lcd_Home_Flag = 1;
      lcd_Menu_Flag = 0;
      lcd_Rec_Flag  = 0;
      lcd_Batt_Flag = 0;
      lcd_LLVD_Flag = 0;

      // đặt lại chỉ mục 
      pos_Rec  = 0; 
      pos_Batt = 0;
      pos_Menu = 0;

      // hiển thị home
      LCD_home();

      // đưa PC về 0
      pos_PC = 0;
    }    
    BT_BACK_Flag = 0;
  }
  else if((BT_BACK_Flag == 1))
  {
    BT_BACK_Flag = 0;
  }  
}

void move_enter(void)
{
  if((BT_ENTER_Flag == 1) && ((pos_PC == 0 ) || (pos_PC == 1)))
  {
    //ECHOLN("BT_ENTER_Flag:"+String(BT_ENTER_Flag));
    switch(pos_PC)
    {
      case 0: // vị trí home- nhấn enter/menu để vào menu
        // hiển thị lcd + mũi tên
        LCD_menu();
        LCD_arrow(0, 1);
        
        // đặt cờ
        lcd_Home_Flag = 0;
        lcd_Menu_Flag = 1;
        lcd_Rec_Flag  = 1;
        lcd_Batt_Flag = 0;
        lcd_LLVD_Flag = 0;

        // đặt PC lên 1
        pos_PC = 1;
        ECHOLN("Menu");
        break;
      case 1: // vị trí menu - dịch chuyển tới các vị trí, nhấn enter/menu để vào từng phần riêng.
        if(lcd_Rec_Flag == 1)
        {
          // hiển thị rect thứ  nhất
          lcd_Rec_Flag  = 1;
          LCD_rectifiers(1, 10, 20, 30, "Normal");
        }
        if(lcd_Batt_Flag == 1)
        {
          //hiển thị batterry thứ nhất 
          lcd_Batt_Flag = 1;
          LCD_battery(1);
        }
        if(lcd_LLVD_Flag == 1)
        {
          lcd_LLVD_Flag = 1;
          if(offset_LLVD != 0)
          {
            EEPROM.writeFloat(addrLLVD, (EEPROM.readFloat(addrLLVD)+offset_LLVD));
            offset_LLVD = 0;
          }
          LCD_LLVD(offset_LLVD);
        }
        lcd_Home_Flag = 0;
        lcd_Menu_Flag = 0;
        
        // đặt PC lên 2.
        pos_PC = 2;
        break;
    }
    BT_ENTER_Flag = 0;
  }
  else if((BT_ENTER_Flag == 1))
  {
    BT_ENTER_Flag = 0;
  } 
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
      DS1307.RTC_getTime(&ACS712.t1_h, &ACS712.t1_m, &ACS712.t1_s); // t1=timenow
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
bool SDCard_saveInfo(char *namefile)
{
  char t[50]=" ";
  float P = 0.0, U = 0.0, I = 0.0;
  float SOH = 0.0, DOD = 0.0;
  
  //DS1307.RTC_getDateTime(t);
  U = getVoltageOfCircuit();
  I = ACS712.getAmpleOfCircuit();
  
  SDCard_getSOH_DOD("SOH_DOD.csv", &SOH, &DOD);
  
  SDMemory.SD_openFile(namefile, FILE_WRITE);
  //SDMemory.SD_writeFile(namefile, "STT,Time,U,I,I1,I2,I3,I4,I5,I6,I7,I8,I9,I10,P,SOH,DOD\n");
  SDMemory.SD_print(INDEX_DATA++);
  
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_writeFile(namefile, t);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_print(U);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_print(I);
  SDMemory.SD_writeFile(namefile, ",");
  
  for(int idex = 56; idex < 66; idex++)
  {
    SDMemory.SD_print(ACS712.getCurrent(idex));
    SDMemory.SD_writeFile(namefile, ",");
  }
  
  SDMemory.SD_print(U*I);
  SDMemory.SD_writeFile(namefile, ",");

  SDMemory.SD_print(SOH);
  SDMemory.SD_writeFile(namefile, ",");
  SDMemory.SD_println(DOD);
  
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
bool SIM_POST_Data(char *url, char *data)
{
  SIM.HTTP_POST_start(url, data);
}

bool SIM_UpDate(char *url, char *data)
{
  // lấy dữ liệu từ thẻ nhớ - bóc tách vv...
  
  // gui du lieu len server
  if(!SIM_POST_Data(url, data)) 
  {
      SDCard_saveInfo("backup.csv"); 
      return false;
  }
  // lưu vào thẻ nhớ
  ECHOLN("\tpush data to server");
  SDCard_saveInfo("data.csv"); 
  return true;
}

bool SIM_Synch(char *url, char *data)
{
  if(!SIM_POST_Data(url, data)) // đồng bộ không thành công 
  {
    return false;
  }  
  // đồng bộ - xóa hàng vừa đồng bộ
  SDCard_clearRow("backup.csv");
  return true;
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
