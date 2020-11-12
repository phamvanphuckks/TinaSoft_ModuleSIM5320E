#include "acs712.h"

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
  float voltage_raw =   (5.0 / 1023.0)* analogRead(_vpin);// Read the voltage from sensor
  
  voltage =  voltage_raw - QOV;
  this->_outVoltage = voltage;
  
  this->_charging = (this->_outVoltage >= 2.5) ? true : false;
  
  this->_current = abs(voltage / FACTOR) - (this->_magnetic_offset_error/1000);
  //this->_current = abs(voltage / FACTOR)/((float)2.5/30.0);
}

boolean ACS712_Sensor::isCharging()
{
  return this->_charging;
}

void ACS712_Sensor::debug(int _vpin)
{
  ECHOLN("=====================");
  ECHO("Sensitivity:");
  ECHO(this->_sensitivity);
  ECHO("Quiesent voltage:");
  ECHOLN(this->_quiescent_Output_voltage);

  ECHOLN("Readings");
  ECHO("\tInput Pin:");
  ECHO(_vpin);
  ECHO("\tSense Voltage:");
  Serial.println(analogRead(_vpin) * 5.0 / 1023.0, 3);

  ECHO("V, Current:");
  ECHO(this->getCurrent(_vpin));
  ECHOLN("A");

  ECHO("\tAverage of 300 readings: ");
  ECHO(getCurrentAverage(_vpin, 300));
  ECHOLN("A");

  ECHO("\is charging :  ");
  ECHOLN(this->_charging);
  ECHOLN("=====================");
} 
