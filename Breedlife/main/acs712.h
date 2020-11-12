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

#define SEN     66.00
#define QV      0.5
#define MOE     0.0
#define VIN     5.0

class ACS712_Sensor
{
  public:
    ACS712_Sensor(float vcc, float sensitivity, float quiescent_Output_voltage, float magnetic_offset_error);
     
    float getCurrent(int _vpin); // gets the current
    float getCurrentAverage(int _vpin, int i); // gets the average of current with number iteration i
    void  debug(int _vpin);
    boolean isCharging();
  private:
    boolean _charging;
    float _vcc, _current, _outVoltage;
    float _sensitivity, _quiescent_Output_voltage, _magnetic_offset_error;   
    
    float readCurrent(int _vpin);
};

#endif
