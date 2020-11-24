/**
 * Author: Tinasoft.vn
 */
#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>

#include "acs712.h"
#include "ds1307.h"
#include "ntc10k.h"
#include "sdcard.h"
#include "lcd1604.h"
#include "sim7600e.h"
#include "ledbutton.h"
#include "timer.h"


#define VOLTAGE     A12

#define T0 0
#define T1 1
#define T2 2

// kí tự mũi tên ->
const byte iArrow[8] = { B00000, B00100, B00110, B11111, B00110, B00100, B00000, B00000 };
const byte ARROW = 0;


typedef enum {SET = 1, CLEAR = !SET} STT;

extern bool lcd_Home_Flag, lcd_Menu_Flag, lcd_Rec_Flag, lcd_Batt_Flag, lcd_LLDV_Flag;
extern int pos_Menu, pos_Rec, pos_Batt, pos_PC;

// hàm khởi tạo các module
void LCD1604_Init();
void DS1307_Init();
void SDCard_Init();
void SIM7600E_Init();
void Led_Button_Buzz_Init();

/*
 * ACS712
*/
IsChargeStatus getIsCharging(void);

unsigned long getTimeSOH(uint8_t t);
float getSOH(unsigned long t1, unsigned long t2);
float getPercentSOH(unsigned long t1, unsigned long t2);
float getVoltageOfCircuit(void);
float getAmpleOfCircuit(void);

unsigned long getDOD(unsigned long t1, unsigned long t2);
void getUIdischarge(void);

/*
 * lcd 
 */
void LCD_displayACS(uint8_t ACSx);
void LCD_home(void);
void LCD_LLDV(void);
void LCD_battery(uint8_t indexBat, uint8_t voltage, uint8_t i, uint8_t imax, char *stt);
void LCD_rectifiers(uint8_t indexRec, uint8_t voltage, uint8_t i, uint8_t imax, char *stt);
void LCD_menu();
void LCD_arrow(uint8_t colum, uint8_t row);

/*
 * DS1307
*/

void DS1307_getTime(uint8_t);

/*
 * SD-Card
*/


/*
 * SIM7600E
*/

bool SIM_POST_Data(char *url, char *data);
/*
 * debug
 */

void ACS712_Debug(void);

#endif /* __APP_H__ */
