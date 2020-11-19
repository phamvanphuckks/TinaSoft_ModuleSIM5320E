#include "acs712.h"
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter fillterACS(1, 1, 0.01);

/*
 * nên sd kalman
*/
ACS712_Sensor::ACS712_Sensor(float vcc, float sensitivity, float quiescent_Output_voltage, float magnetic_offset_error)
{
  this->_vcc = vcc;
  this->_sensitivity = sensitivity;
  this->_quiescent_Output_voltage = quiescent_Output_voltage;
  this->_magnetic_offset_error = magnetic_offset_error;
}

float ACS712_Sensor::getCurrentAverage(int _vpin, int i)
{
  float sum = 0, average;

  for(int index = 0; index < i; index++)
  {
    sum += getCurrent(_vpin);
  }
  
  average = sum /i;
  return average;
}

float ACS712_Sensor::getCurrent(int _vpin)
{
  readCurrent(_vpin);
  return this->_current;
} 

float ACS712_Sensor::readCurrent(int _vpin)
{
  const float FACTOR = this->_sensitivity/1000;// set sensitivity for selected model - unit V
  const float QOV    = this->_quiescent_Output_voltage * this->_vcc;// set quiescent Output voltage for selected model
  
  float voltage;// internal variable for voltage
  float voltageOut  = fillterACS.updateEstimate(analogRead(_vpin));
  float voltage_raw =   (5.0 / 1023.0)* voltageOut;// Read the voltage from sensor
  //float voltage_raw =   (5.0 / 1023.0)* voltageOut;// Read the voltage from sensor
  // quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
  // kiểm tra đang sạc không
  if(voltage_raw > QOV + OFFSET_SEN)
  {
    this->_charging =  DISCHARGE;
  }
  else if(voltage_raw < QOV - OFFSET_SEN)
  {
    this->_charging =  CHARGE;
  }
  else
  {
    this->_charging =  NONE;
  }
  
  voltage =  voltage_raw - QOV;
  this->_outVoltage = voltage;
  
  this->_current = abs(voltage / FACTOR) - (this->_magnetic_offset_error/1000);
}

IsChargeStatus ACS712_Sensor::isCharging()
{
  return this->_charging;
}

float ACS712_Sensor::getAmpleOfCircuit(void)
{
  return (getCurrent(ACS1)+getCurrent(ACS2)+getCurrent(ACS3)+getCurrent(ACS4)+getCurrent(ACS5)+getCurrent(ACS6)+getCurrent(ACS7)+getCurrent(ACS8)+getCurrent(ACS9)+getCurrent(ACS10)); 
}

void ACS712_Sensor::debug(int _vpin)
{
  ECHOLN("=========================================");
  ECHO("Sensitivity: ");
  ECHO(this->_sensitivity);
  ECHOLN(" mV");
  ECHO("Quiesent voltage: ");
  ECHO(this->_quiescent_Output_voltage);
  ECHOLN(" V");

  ECHOLN("Readings....");
  ECHO("\tInput Pin: ");
  ECHOLN(_vpin);
  ECHO("\tVoltage: ");
  Serial.print(analogRead(_vpin) * 5.0 / 1023.0, 3);
  ECHOLN(" V");
  
  ECHO("\tCurrent: ");
  ECHO(this->getCurrent(_vpin));
  ECHOLN(" A");

  ECHO("\tAverage of 200 readings: ");
  ECHO(getCurrentAverage(_vpin, 200));
  ECHOLN(" A");

  ECHO("\tIs charging : ");
  ECHOLN(this->_charging);
  ECHOLN("=========================================");
} 
