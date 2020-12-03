/**
 * Author: Tinasoft.vn
 */
#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>
#include <EEPROMex.h>

#include "acs712.h"
#include "ds1307.h"
#include "ntc10k.h"
#include "sdcard.h"
#include "lcd1604.h"
#include "sim7600e.h"
#include "ledbutton.h"
#include "timer.h"

/*address EEPROM*/
#define addrPosDATA   0
#define addrPosBACKUP 4
#define addrLLVD      8

#define VOLTAGE     A12

#define T0 0
#define T1 1
#define T2 2

/*lcd 1604*/
#define LCD_POWER 22
// kí tự mũi tên ->
const byte iArrow[8] = { B00000, B00100, B00110, B11111, B00110, B00100, B00000, B00000 };
const byte ARROW = 0;

typedef enum {SET = 1, CLEAR = !SET} STT;

enum lcd1604Flag {
  HOME_Flag   = 1,
  MENU_Flag   = 2,
  REC_Flag    = 4,
  BAT_Flag    = 8,
  LLVD_Flag   = 16
};
extern uint8_t lcdFlag;

extern int pos_Menu, pos_Rec, pos_Batt;
extern float LLVD_value, offset_LLVD;
extern uint32_t INDEX_DATA, INDEX_BACKUP;
extern float Psum;


extern bool Battery_Flag;


typedef struct Data { 
      char url[50];        
      char data[100];                
      float acs[11];
      float U;
      float V;     
      float P; 
      float SOH;
      float DOD;                  
} DataSIM;

/*
 * Hàm khởi tạo các module
 */
void LCD1604_Init();
void DS1307_Init();
void SDCard_Init();
void SIM7600E_Init();
void Led_Button_Buzz_Init();

/*
 * ACS712
*/
extern bool t0_Flag, t1_Flag, t2_Flag;
IsChargeStatus getIsCharging(void);

unsigned long getTimeSOH(uint8_t t);
float getSOH(unsigned long t1, unsigned long t2);
float getPercentSOH(unsigned long t1, unsigned long t2);
float getVoltageOfCircuit(void);
float getAmpleOfCircuit(void);

unsigned long getDOD(unsigned long t1, unsigned long t2);
void getUIdischarge(void);
void Calulation_SOH_DOD(void);

/*
 * lcd1604
 */
void LCD_home(void);
void LCD_LLVD(float offset);
void LCD_battery(uint8_t indexBat);
void LCD_rectifiers(uint8_t indexRec, uint8_t voltage, uint8_t i, uint8_t imax, char *stt);
void LCD_menu();
void LCD_arrow(uint8_t colum, uint8_t row);

void move_up();
void move_down();
void move_back();
void move_enter();
/*end-lcd1604*/

/*
 * DS1307
*/
void DS1307_getTime(uint8_t);

/*
 * SD-Card
*/

bool SDCard_saveInfo(char *namefile);
bool SDCard_saveSOH_DOD(char *namefile, float SOH, float DOD);
bool SDCard_getSOH_DOD(char *namefile, float *SOH, float *DOD);
bool SDCard_saveInfo(char *namefile, DataSIM *data);
bool SDCard_getDataBackUp(char *namefile, DataSIM *data);

bool SDCard_clearRow(char *namefile);
/*
 * NTC-10k
*/
float getTemperture(uint8_t ntc);

/*
 * SIM7600E
*/
bool SIM_POST_Data(char *url, char *data);
bool SIM_UpDate(char *url, char *data);
bool SIM_Synch(char *url, char *data);

/*
 * debug
 */
void ACS712_Debug(int ACSx);

/*
 * backup
*/
void back_up(void);
#endif /* __APP_H__ */
