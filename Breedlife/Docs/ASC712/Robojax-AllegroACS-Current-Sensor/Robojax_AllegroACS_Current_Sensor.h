/*
 * File: Robojax_AllegroACS_Current_Sensor.h
Written by Ahmad Shamshiri (Robojax.com) on Mar 15, 2020
Updated to April 04, 2020 (see version log)

 * Description: This is for Allegro ACS linear Hall Effect Sensor.

 * Author: Ahmad Shamshiri
 * Contact: robojax.com
 * Copyright: 2020 Ahmad Shamshiri



    Robojax_AllegroACS_Current_Sensor.h s free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Update logs	
2020-Apr-04
		magnetic_offset_error was considered in calculations
		VCC example was added
2020-Mar-15 Initial wiring of this libarary	
*/

#ifndef Robojax_AegroAcs_Current_sensor_H
#define Robojax_AegroAcs_Current_sensor_H

#include "Arduino.h"

class Robojax_AllegroACS_Current_Sensor
{
public:
    char *_version = "1.1-2020-Apr-04";
    //Arduino board
    Robojax_AllegroACS_Current_Sensor(int model, int vinPin);

    // with VCC defined
    Robojax_AllegroACS_Current_Sensor(int model, int vinPin, float vcc);

    /*
	* gets the current 
	*/
    float getCurrent();

    //gets the average of current with number iteration i
    float getCurrentAverage(int i);

    //prints detailed debugging information about the module
    void debug();

private:
    /**
     * read current
     */
    float readCurrent();

    int _vin, _model;
    float _vcc, _current, _outVoltage;
    static float _sensitivity[];
    static float _quiescent_Output_voltage[];
    static float _magnetic_offset_error[];

    static char *_modelNames[];
};

#endif
