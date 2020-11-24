#ifndef __NTC10k_H__
#define __NTC10k_H__

#include <Arduino.h>
#include "AppDebug.h"

#define NTC10kpin1  A0
#define NTC10kpin2  A1

/*
 * thermistor parameters:
 * RT0: 10000 Ω
 * B: 3977 K +- 0.75%
 * T0:  25 C
 * +- 5%
 */

//These values are in the datasheet
#define RT0 10000.0   // Ω
#define B 3977      // K

#define VCC 5.0    //Supply voltage
#define R 10000.0  //R=10KΩ


class NTC10kSensor
{
  public:
    NTC10kSensor();
    float NTC10k_ReadTemperture(int pinNTC10k);
  private:
    float _RT, _VR, _ln, _Tc, _T0, _VRT, _Vo;
};

#endif
