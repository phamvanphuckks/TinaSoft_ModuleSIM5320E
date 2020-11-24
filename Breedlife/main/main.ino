#include <Arduino.h>
#include "App.h"

bool t0_Flag = 0, t1_Flag = 0, t2_Flag = 0;
volatile bool BT_UP_Flag = 0, BT_DOWN_Flag = 0, BT_BACK_Flag = 0, BT_ENTER_Flag = 0; 
bool lcd_Home_Flag = 0, lcd_Menu_Flag = 0, lcd_Rec_Flag = 0, lcd_Batt_Flag = 0, lcd_LLDV_Flag = 0;
int pos_Menu = 0, pos_Rec = 0, pos_Batt = 0, pos_PC = 0;
volatile uint8_t SIM_Flag = 0;
volatile uint16_t Sync_Flag = 0;

void setup()
{
  Serial.begin(9600);
  // initial lcd
  LCD1604_Init();
  // LED-Buzz-Butotn
  Led_Button_Buzz_Init();
  // RTC-DS1307
  DS1307_Init();
//  // SD-Card
//  SDCard_Init();
  // Module SIM
  SIM7600E_Init();
}

void loop()
{
//  ACS712_Debug();
//  delay(2000);

//  /* 
//   * khi bộ sạc được cắm điện : bộ sạc vừa sạc cho accuy vừa cấp nguồn cho tải.
//   * Khi mà accuy đầy :  bộ sạc chỉ cung cấp cho tải <=> ACS712 đọc được 0A +- sai số 0.2A, U=48V.
//   * (getVoltage() >= 40.0) : Loại bỏ trường hợp bị mất điện.
//   * (ACS712.getCurrent(ACS1) == 0.0) : mạch không sạc, không xả <=> đang cắm sạc, và bình accuy đầy.
//   * (t1_Flag == 0) : Không lặp lại ghi t0 vào thẻ nhớ khi accuy đầy
//  */  
//
//  if((t0_Flag == CLEAR) && (getIsCharging() == NONE) && (getVoltageOfCircuit() >= 40.0)) 
//  {
//    ECHOLN("Phat hien mach khong sac, xa : t0");
//    DS1307_getTime(T0);// t0=timenow
//    // ghi t0 vào thẻ nhớ
//    
//    t0_Flag = SET;
//    t1_Flag = SET;
//    t2_Flag = CLEAR;
//  }
//
//  /*
//   * Nếu mà phát hiện ra đang sạc đầy và nhận thấy quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
//   * t1_Flag == 0) : Không lặp lại quá trình ghi T2
//   * (getVoltageOfCircuit() < 40.0) : không cắm sạc,lúc này accuy xả cho tải.
//  */
//  if((t1_Flag == SET) && (getIsCharging() == DISCHARGE) && (getVoltageOfCircuit() < 40.0))
//  {
//    ECHOLN("Phat hien mach xa : t1");
//    DS1307_getTime(T1);// t1=timenow
//    // ghi t1 vào SD card
//    
//    // thiếu đọc liên tục cá giá trị dòng điện và điện áp vào SD card
//
//    // lấy u xả và i xả
//    getUIdischarge();
//
//    // set cờ tính toán SOH
//    t0_Flag = SET;
//    t1_Flag = CLEAR;
//    t2_Flag = SET;
//  }
//
//  /*
//   * Nếu đang xả của quá trình đo thời gian xả đế tính SOH và DOD -> căm sạc vào , hủy bỏ lần tính toán đấy.
//   * => không tính lần SOH, DOD đấy.
//  */
//  if((t2_Flag == SET) && (getIsCharging() == CHARGE) && (getVoltageOfCircuit() >= 40.0))
//  {
//    ECHOLN("Phat hien mach xa, bi cam xac");
//
//    // xóa cờ phục vụ chu kì mới
//    t0_Flag = CLEAR;
//    t1_Flag = CLEAR;
//    t2_Flag = CLEAR;
//  }
//
//  /*
//   * I=0, U<40 => bình cạn
//   * Tính SOH và DOD
//  */
//  if( (t2_Flag == SET) && (getIsCharging() == NONE) && (getVoltageOfCircuit() < 40))// bình cạn
//  {
//    ECHOLN("Phat hien mach xa het, tinh toan SOH, DOD: t2");
//    unsigned long t1 = 0, t2 = 0;
//   
//    DS1307_getTime(T2);// t2=timenow
//    t1 = getTimeSOH(T1);
//    t2 = getTimeSOH(T2);
//
//    // tính toán SOH,DOC
//    getSOH(t1, t2);
//    getDOD(t1, t2);
//
//    // xóa cờ, thực hiện chu kì tính toán SOH, DOD mới.
//    t0_Flag = CLEAR;
//    t1_Flag = CLEAR;
//    t2_Flag = CLEAR;
//  }
//
//  /*
//   * I=0, U>=40 bình ko cạn, nhưng bị rút tải
//   * không tính gì cả, chỉ lưu lại giá trị t2 tính DOD
//  */
//  else if( (t2_Flag == SET) && (getIsCharging() == NONE) && (getVoltageOfCircuit() >= 40.0))
//  {
//    ECHOLN("Phat hien mach xa het, nhung bi rut tai");
//    unsigned long t1=0, t2=0;
//   
//    DS1307_getTime(T2);// t2=timenow
//    t1 = getTimeSOH(T1);
//    t2 = getTimeSOH(T2);
//
//    // tính toán DOD
//    getDOD(t1, t2);
//
//    // xóa cờ thực hiện chu kì mới
//    t0_Flag = CLEAR;
//    t1_Flag = CLEAR;
//    t2_Flag = CLEAR;
//  }

  /*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  
  /*
   * Xử lý LCD - button
  */
  
  // up
  if((BT_UP_Flag == 1) && (lcd_Menu_Flag == 1)) // up menu
  {
    pos_Menu++;
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
        lcd_LLDV_Flag = 0;

        // mũi tên
        LCD_menu();
        LCD_arrow(0, 1);
        break;
      case 1:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 1;
        lcd_LLDV_Flag = 0;

        // mũi tên
        LCD_menu();
        LCD_arrow(0, 2);
        break;
      case 2:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 0;
        lcd_LLDV_Flag = 1; 
        
        // mũi tên
        LCD_menu();
        LCD_arrow(0, 3);     
        break;
      default:
        break;
    }
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1) && (lcd_Rec_Flag == 1)) // up rectifiers
  {
    pos_Rec++;
    
    if(pos_Rec > 9)
    {
      pos_Rec = 0;
    }
    ECHOLN("pos_Rec:"+String(pos_Rec));
    LCD_rectifiers(pos_Rec+1, 8, 10,30,"Normal");
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1) && (lcd_Batt_Flag == 1)) // up battery
  {
    pos_Batt++;
    if(pos_Batt > 9)
    {
      pos_Batt = 0;
    }
    ECHOLN("pos_Batt:"+String(pos_Batt));
    LCD_battery(pos_Batt+1, 5, 7,20, "Normal");
    BT_UP_Flag = 0;
  }
  else if((BT_UP_Flag == 1))
  {
    BT_UP_Flag = 0;
  }

  // down
  if((BT_DOWN_Flag == 1) && (lcd_Menu_Flag == 1)) //  down menu
  {
    pos_Menu--;
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
        lcd_LLDV_Flag = 0;

        // mũi tên
        LCD_menu();
        LCD_arrow(0, 1);
        break;
      case 1:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 1;
        lcd_LLDV_Flag = 0;

        // mũi tên
        LCD_menu();
        LCD_arrow(0, 2);
        break;
      case 2:
        lcd_Home_Flag = 0;
        lcd_Rec_Flag  = 0;
        lcd_Batt_Flag = 0;
        lcd_LLDV_Flag = 1;

        // mũi tên
        LCD_menu();
        LCD_arrow(0, 3);
        break;
      default:
        break;
    }
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1)  && (lcd_Rec_Flag == 1)) // down rectifiers
  {
    pos_Rec--;
    if(pos_Rec < 0)
    {
      pos_Rec = 9;
    }
    ECHOLN("pos_Rec:"+String(pos_Rec));
    LCD_rectifiers(pos_Rec+1, 10, 20,30, "Normal");
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1)  && (lcd_Batt_Flag == 1))// down batteries
  {
    pos_Batt--;
    if(pos_Batt < 0)
    {
      pos_Batt = 9;
    }
    ECHOLN("pos_Batt:"+String(pos_Batt));
    LCD_battery(pos_Batt+1, 7, 20, 30,"Normal");
    BT_DOWN_Flag = 0;
  }
  else if((BT_DOWN_Flag == 1))
  {
    BT_DOWN_Flag = 0;
  }
  
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
      lcd_LLDV_Flag = 0;

      // hiển thị menu
      LCD_menu();
      // mũi tên
      LCD_menu();
      LCD_arrow(0, 1);     

      // đặt lại chỉ mục 
      pos_Rec  = 0; 
      pos_Batt = 0;
      pos_Menu = 0;

      // đưa PC về 1
      pos_PC = 1;
    }
    else if(pos_PC == 1) // từ menu quay về home
    {
      // đặt lại cờ
      lcd_Home_Flag = 1;
      lcd_Menu_Flag = 0;
      lcd_Rec_Flag  = 0;
      lcd_Batt_Flag = 0;
      lcd_LLDV_Flag = 0;

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

  // enter/menu
  if((BT_ENTER_Flag == 1) && ((pos_PC == 0 )|| (pos_PC == 1)))
  {
    //ECHOLN("BT_ENTER_Flag:"+String(BT_ENTER_Flag));
    switch(pos_PC)
    {
      case 0: // vị trí home- nhấn enter/menu để vào menu
        // hiển thị lcd
        LCD_menu();
        // mũi tên
        LCD_menu();
        LCD_arrow(0, 1);
        // đặt cờ
        lcd_Home_Flag = 0;
        lcd_Menu_Flag = 1;
        lcd_Rec_Flag  = 1;
        lcd_Batt_Flag = 0;
        lcd_LLDV_Flag = 0;

        // đặt PC lên 1
        pos_PC = 1;
        ECHOLN("Menu");
        break;
      case 1: // vị trí menu - dịch chuyển tới các vị trí, nhấn enter/menu để vào từng phần riêng.
        if(lcd_Rec_Flag == 1)
        {
          // hiển thị rect thứ  nhất
          lcd_Rec_Flag  = 1;
          LCD_rectifiers(1,10,20,30,"Normal" );
        }
        if(lcd_Batt_Flag == 1)
        {
          //hiển thị rect thứ nhất 
          lcd_Batt_Flag = 1;
          LCD_battery(1,10,20,30,"Normal");
        }
        if(lcd_LLDV_Flag == 1)
        {
          lcd_LLDV_Flag = 1;
          LCD_LLDV();
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

  /*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  /* 
   * module SIM7600E - 1p truy vấn lại
   */

//  static const char *url = "http://admin.breedlife.com/api/v1/data";
//  static const char *data = "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}";
//  
//  if(SIM_Flag == 5)
//  {
//    // gui du lieu len server
//    if(!SIM_POST_Data(url, data)) // nếu gửi không thành công ghi vào key lỗi
//    {
//      
//    }
//    // lưu vào thẻ nhớ
//    ECHOLN("push data to server");
//    SIM_Flag = 0;
//  }
//
//  // gửi lại những lần bị lỗi - 1h kiểm tra xem có cái nào gửi lỗi - gửi lại
//  if(Sync_Flag == 3600)
//  {
//
//    Sync_Flag = 0;
//  }
}
