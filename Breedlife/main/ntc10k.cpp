#include "ntc10k.h"

NTC10kSensor::NTC10kSensor()
{
  _T0 = 25 + 273.15; 
}

float NTC10kSensor::NTC10k_ReadTemperture(int pinNTC10k)
{
  _Vo = analogRead(pinNTC10k);
  _VRT = (5.00 / 1023.00) * _Vo;      //Conversion to voltage
  _VR = VCC - _VRT;
  _RT = _VRT / (_VR / R);               //Resistance of RT
  _ln = log(_RT / RT0);
  _Tc = (1 / ((_ln / B) + (1 / _T0))); //Temperature from thermistor
  _Tc = _Tc - 273.15;                 //Conversion to Celsius
 
  ECHOLN("tempertre");
  ECHOLN(_Tc);
  return _Tc;
}
