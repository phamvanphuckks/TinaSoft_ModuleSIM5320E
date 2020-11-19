#include <Arduino.h>
#include "App.h"

ACS712_Sensor ACS712(VIN, SEN, QV, MOE);

////note  old version 0X27, new version 0x3F
LDC1604Display LDC1604(0x27, 16, 4);

DS1307Time DS1307;

NTC10kSensor NTC10k;

SDCard  SDMemory;

SIM7600E  SIM7600E_SS;

bool t0_Flag = 0, t1_Flag = 0, t2_Flag = 0;

void setup()
{
  Serial.begin(9600);

//  // initial lcd
//  LDC1604.init();                    
//  LDC1604.backlight();
//  
//  // LED-Buzz-Butotn
//  Init_Buzz();
//  Init_LED();
//  Init_Button();
//
//  // RTC-DS1307
//  DS1307.RTC_Init();
//
//  // SD-Card
//  SDMemory.SD_Init();
//  
//  // NTC10k
//  NTC10k.NTC10k_ReadTemperture(NTC10kpin1);
}

void loop()
{
//  ACS712.debug(ACS1);
//  ECHOLN(getVoltage());
//  delay(2000);

  /* 
   * khi bộ sạc được cắm điện : bộ sạc vừa sạc cho accuy vừa cấp nguồn cho tải.
   * Khi mà accuy đầy :  bộ sạc chỉ cung cấp cho tải <=> ACS712 đọc được 0A +- sai số 0.2A, U=48V.
   * (getVoltage() >= 40.0) : Loại bỏ trường hợp bị mất điện.
   * (ACS712.getCurrent(ACS1) == 0.0) : mạch không sạc, không xả <=> đang cắm sạc, và bình accuy đầy.
   * (t1_Flag == 0) : Không lặp lại ghi t0 vào thẻ nhớ khi accuy đầy
  */  

  if((t0_Flag == 0) && (ACS712.isCharging() == NONE) && (getVoltageOfCircuit() >= 40.0)) 
  {
    ECHOLN("Phat hien mach khong sac, xa : t0");
    DS1307.RTC_getTime(&ACS712.t0_h, &ACS712.t0_m, &ACS712.t0_s); // t0=timenow
    // ghi t0 vào thẻ nhớ
    
    t0_Flag = 1;
    t1_Flag = 1;
    t2_Flag = 0;
  }

  /*
   * Nếu mà phát hiện ra đang sạc đầy và nhận thấy quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
   * t1_Flag == 0) : Không lặp lại quá trình ghi T2
   * (getVoltageOfCircuit() < 40.0) : không cắm sạc,lúc này accuy xả cho tải.
  */
  if((t1_Flag == 1) && (ACS712.isCharging() == DISCHARGE) && (getVoltageOfCircuit() < 40.0))
  {
    ECHOLN("Phat hien mach xa : t1");
    DS1307.RTC_getTime(&ACS712.t1_h, &ACS712.t1_m, &ACS712.t1_s); // t0=timenow
    // ghi t1 vào SD card
    
    // thiếu đọc liên tục cá giá trị dòng điện và điện áp vào SD card

    // lấy u xả và i xả
    ACS712.u_discharge = getVoltageOfCircuit();
    ACS712.i_discharge = ACS712.getAmpleOfCircuit();

    // set cờ tính toán SOH
    t0_Flag = 1;
    t1_Flag = 0;
    t2_Flag = 1;
  }

  /*
   * Nếu đang xả của quá trình đo thời gian xả đế tính SOH và DOD -> căm sạc vào , hủy bỏ lần tính toán đấy.
   * => không tính lần SOH, DOD đấy.
  */
  if((t2_Flag == 1) && (ACS712.isCharging() == CHARGE) && (getVoltageOfCircuit() >= 40.0))
  {
    ECHOLN("Phat hien mach xa, bi cam xac");

    // xóa cờ phục vụ chu kì mới
    t0_Flag = 0;
    t1_Flag = 0;
    t2_Flag = 0;
  }

  /*
   * I=0, U<40 => bình cạn
   * Tính SOH và DOD
  */
  if( (t2_Flag == 1) && (ACS712.isCharging() == NONE) && (getVoltageOfCircuit() < 40))// bình cạn
  {
    ECHOLN("Phat hien mach xa het, tinh toan SOH, DOD: t2");
    unsigned long t1 = 0, t2 = 0;
   
    DS1307.RTC_getTime(&ACS712.t2_h, &ACS712.t2_m, &ACS712.t2_s); // t2=timenow
    t1 = ACS712.t1_h*60*ACS712.t1_m*60 + ACS712.t1_s;
    t2 = ACS712.t2_h*60*ACS712.t2_m*60 + ACS712.t2_s;

    // tính toán SOH,DOC
    getSOH(t1, t2, ACS712.u_discharge, ACS712.i_discharge);
    getDOD(t1, t2);

    // xóa cờ, thực hiện chu kì tính toán SOH, DOD mới.
    t0_Flag = 0;
    t1_Flag = 0;
    t2_Flag = 0;
  }

  /*
   * I=0, U>=40 bình ko cạn, nhưng bị rút tải
   * không tính gì cả, chỉ lưu lại giá trị t2 tính DOD
  */
  else if( (t2_Flag == 1) && (ACS712.isCharging() == NONE) && (getVoltageOfCircuit() >= 40.0))
  {
    ECHOLN("Phat hien mach xa het, nhung bi rut tai");
    unsigned long t1=0, t2=0;
   
    DS1307.RTC_getTime(&ACS712.t2_h, &ACS712.t2_m, &ACS712.t2_s); // t2=timenow
    t1 = ACS712.t1_h*60*ACS712.t1_m*60 + ACS712.t1_s;
    t2 = ACS712.t2_h*60*ACS712.t2_m*60 + ACS712.t2_s;

    // tính toán DOD
    getDOD(t1, t2);

    // xóa cờ thực hiện chu kì mới
    t0_Flag = 0;
    t1_Flag = 0;
    t2_Flag = 0;
  }
}
