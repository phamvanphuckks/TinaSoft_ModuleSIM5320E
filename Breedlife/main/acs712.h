#ifndef __ACS712_H__
#define __ACS712_H__

#include <Arduino.h>
#include "AppDebug.h"

#define ACS1    A2
#define ACS2    A3
#define ACS3    A4
#define ACS4    A5
#define ACS5    A6
#define ACS6    A7
#define ACS7    A8
#define ACS8    A9
#define ACS9    A10
#define ACS10   A11

#define SEN     66.0
#define QV      0.5
#define MOE     0.0 // sửa tham số này để bù lỗi lỗi giữa đồng hồ đo và giá trị đọc được <=> hiệu chỉnh, do thông số nhà sản xuất không chính xác.
#define VIN     5.0

// cách calibartion

/* 
 *  P1(2.527, 0) : đo điện áp tại điểm Vout, không có tải => P1
 * P2(2.687, 1.155) : dùng đồng hồ đo -> P2
 *  => sensitivity = V2-V1/I2-I1=2.687-2.527/1.155-0 = 0.139 V/A
*/

#define OFFSET_SEN 0.02

typedef enum {CHARGE = 0, DISCHARGE = !CHARGE, NONE=2} IsChargeStatus;


class ACS712_Sensor
{
  public:
    ACS712_Sensor(float vcc, float sensitivity, float quiescent_Output_voltage, float magnetic_offset_error);
     
    float getCurrent(int _vpin); // gets the current
    float getCurrentAverage(int _vpin, int i); // gets the average of current with number iteration i
    void  debug(int _vpin);
    float getAmpleOfCircuit();
    IsChargeStatus isCharging();

    unsigned int t0_h = 0, t0_m = 0, t0_s = 0, t1_h = 0, t1_m = 0, t1_s = 0, t2_h = 0, t2_m = 0, t2_s = 0;
    unsigned int u_discharge, i_discharge;
  private:
    IsChargeStatus _charging;
    float _vcc, _current, _outVoltage;
    float _sensitivity, _quiescent_Output_voltage, _magnetic_offset_error;   
    float readCurrent(int _vpin);
};

#endif
