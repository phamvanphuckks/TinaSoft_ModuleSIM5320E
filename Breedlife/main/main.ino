#include <Arduino.h>
#include "App.h"

bool t0_Flag = 0, t1_Flag = 0, t2_Flag = 0;
volatile bool BT_UP_Flag = 0, BT_DOWN_Flag = 0, BT_BACK_Flag = 0, BT_ENTER_Flag = 0; 
bool lcd_Home_Flag = 0, lcd_Menu_Flag = 0, lcd_Rec_Flag = 0, lcd_Batt_Flag = 0, lcd_LLVD_Flag = 0;
int pos_Menu = 0, pos_Rec = 0, pos_Batt = 0, pos_PC = 0;
int LLVD_value = 0;
float offset_LLVD = 0.0;

volatile uint8_t  SIM_Flag = 0;
volatile uint16_t TIM_UpdateCurrent = 0;
volatile uint16_t Sync_Flag = 0;
uint32_t INDEX_DATA = 0, INDEX_BACKUP = 0;

void setup()
{
  Serial.begin(9600);

//  SIM7600E_Init(); // Module SIM
//  LCD1604_Init(); // initial lcd
//  Led_Button_Buzz_Init(); // LED-Buzz-Butotn
//  DS1307_Init(); // RTC-DS1307
//  SDCard_Init(); // SD-Card
}

void loop()
{
//  Calulation_SOH_DOD();
//  
//  /*
//   * Xử lý LCD - button
//  */
//  move_up();
//  move_down();
//  move_back();
//  move_enter();
//  
//  // 5s update - battery
//  if((lcd_Batt_Flag == 1) && TIM_UpdateCurrent > 10)
//  {
//    LCD_battery(pos_Batt+1);
//    TIM_UpdateCurrent = 0;
//  }
//  // 5s update - home
//  if((lcd_Home_Flag == 1) && TIM_UpdateCurrent > 10)
//  {
//    LCD_home();
//    TIM_UpdateCurrent = 0;
//  }  
//  
//  /* 
//   * module SIM7600E - 1p truy vấn lại
//   */
//  static const char *url = "http://admin.breedlife.com/api/v1/data";
//  static const char *data = "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}";
//  
//  if(SIM_Flag == 60)
//  {
//    SIM_UpDate(url, data);
//    SIM_Flag = 0;
//  }
//  // gửi lại những lần bị lỗi - 1h kiểm tra xem có cái nào gửi lỗi - gửi lại
//  if(Sync_Flag == 3600)
//  {
//    SIM_Synch(url, data);
//    Sync_Flag = 0;
//  }


}
