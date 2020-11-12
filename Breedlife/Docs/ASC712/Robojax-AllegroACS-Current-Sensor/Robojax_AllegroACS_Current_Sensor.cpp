/**
 * File: Robojax_AllegroACS_Current_Sensor.cpp
Written by Ahmad Shamshiri (Robojax.com) on Mar 15, 2020
Updated to April 04, 2020 (see update log)

 * Description: This is for Allegro ACS linear Hall Effect Sensor.

 * Author: Ahmad Shamshiri
 * Contact: robojax.com
 * Copyright: 2020 Ahmad Shamshiri



    Robojax_AllegroACS_Current_Sensor.cpp is free software: you can redistribute it and/or modify
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
2020-Mar-15 Initial wiring of this libarary	
*/

#include "Arduino.h"
#include "Robojax_AllegroACS_Current_Sensor.h"

char *Robojax_AllegroACS_Current_Sensor::_modelNames[] = {
	//ACS712
	"ACS712ELCTR-05B",
	"ACS712ELCTR-20A",
	"ACS712ELCTR-30A",

	//ACS758
	"ACS758LCB-050B",
	"ACS758LCB-050U",
	"ACS758LCB-100B",
	"ACS758LCB-100U",
	"ACS758KCB-150B",
	"ACS758KCB-150U",
	"ACS758ECB-200B",
	"ACS758ECB-200U",

	//ACS770
	"ACS770x-050B", //11
	"ACS770x-050U",
	"ACS770x-100B",
	"ACS770x-100U",
	"ACS770x-150B",
	"ACS770x-150U",
	"ACS770x-200B",
	"ACS770x-200U",

	//ACS732
	"ACS732KLATR-20AB",
	"ACS732KLATR-40AB",
	"ACS732KLATR-65AB",
	"ACS732KLATR-65AU",
	"ACS732KLATR-75AB",

	"ACS733KLATR-20AB",
	"ACS733KLATR-40AB",
	"ACS733KLATR-40AU",
	"ACS733KLATR-65AU",

};
float Robojax_AllegroACS_Current_Sensor::_sensitivity[] = {

	185.0,	//ACS712ELCTR-05B
	100.00, //ACS712ELCTR-20A
	66.00,	//ACS712ELCTR-30A

	//ACS758
	40.0, // for ACS758LCB-050B
	60.0, // for ACS758LCB-050U
	20.0, // for ACS758LCB-100B
	40.0, // for ACS758LCB-100U
	13.3, // for ACS758KCB-150B
	16.7, // for ACS758KCB-150U
	10.0, // for ACS758ECB-200B
	20.0, // for ACS758ECB-200U

	//ACS770
	40.0, // for ACS770x-050B ///	11
	80.0, // for ACS770x-050U
	20.0, // for ACS770x-100B
	40.0, // for ACS770x-100U
	13.3, // for ACS770x-150B
	26.7, // for ACS770x-150U
	10.0, // for ACS770x-200B
	20.0, // for ACS770x-200U

	100.0, //for "ACS732KLATR-20AB",
	50.0,  // for "ACS732KLATR-40AB",
	30.0,  // for "ACS732KLATR-65AB",
	60.0,  // for "ACS732KLATR-65AU",
	26.6,  // for "ACS732KLATR-75AB",

	66.0, // for "ACS733KLATR-20AB",
	33.0, // for "ACS733KLATR-40AB",
	66.0, // for "ACS733KLATR-40AU",
	20.0, // for "ACS733KLATR-65AU",

};

float Robojax_AllegroACS_Current_Sensor::_quiescent_Output_voltage[] = {
	0.5, //ACS712ELCTR-05B
	0.5, //ACS712ELCTR-20A
	0.5, //ACS712ELCTR-30A

	0.5,  // for ACS758LCB-050B
	0.12, // for ACS758LCB-050U
	0.5,  // for ACS758LCB-100B
	0.12, // for ACS758LCB-100U
	0.5,  // for ACS758KCB-150B
	0.12, // for ACS758KCB-150U
	0.5,  // for ACS758ECB-200B
	0.12, // for ACS758ECB-200U

	0.5, // for ACS770x-050B ///
	0.1, // for ACS770x-050U
	0.5, // for ACS770x-100B
	0.1, // for ACS770x-100U
	0.5, // for ACS770x-150B
	0.1, // for ACS770x-150U
	0.5, // for ACS770x-200B
	0.1, // for ACS770x-200U

	0.5, //for "ACS732KLATR-20AB",
	0.5, // for "ACS732KLATR-40AB",
	0.5, // for "ACS732KLATR-65AB",
	0.1, // for "ACS732KLATR-65AU",
	0.5, // for "ACS732KLATR-75AB",

	0.5, // for "ACS733KLATR-20AB",
	0.5, //for"ACS733KLATR-40AB",
	0.1, // for "ACS733KLATR-40AU",
	0.1, // for "ACS733KLATR-65AU",

};
float Robojax_AllegroACS_Current_Sensor::_magnetic_offset_error[] = {
	0.0, //ACS712ELCTR-05B
	0.0, //ACS712ELCTR-20A
	0.0, //ACS712ELCTR-30A

	0.0,  // for ACS758LCB-050B
	0.12, // for ACS758LCB-050U
	0.5,  // for ACS758LCB-100B
	0.12, // for ACS758LCB-100U
	0.5,  // for ACS758KCB-150B
	0.12, // for ACS758KCB-150U
	0.5,  // for ACS758ECB-200B
	0.12, // for ACS758ECB-200U

	120.0, // for ACS770x-050B ///
	120.0, // for ACS770x-050U
	170.0, // for ACS770x-100B
	170.0, // for ACS770x-100U
	225.0, // for ACS770x-150B
	225.0, // for ACS770x-150U
	250.0, // for ACS770x-200B
	250.0, // for ACS770x-200U

	0.0, //for "ACS732KLATR-20AB",
	0.0, // for "ACS732KLATR-40AB",
	0.0, // for "ACS732KLATR-65AB",
	0.0, // for "ACS732KLATR-65AU",
	0.0, // for "ACS732KLATR-75AB",

	0.0, // for "ACS733KLATR-20AB",
	0.0, //for"ACS733KLATR-40AB",
	0.0, // for "ACS733KLATR-40AU",
	0.0, // for "ACS733KLATR-65AU",

};
// -- constructor without VCC
Robojax_AllegroACS_Current_Sensor::Robojax_AllegroACS_Current_Sensor(int model, int vinPin)
{
	// -- value
	_vin = vinPin;
	_model = model;
	_vcc = 5.0;
}

// -- constructor without with vcc
Robojax_AllegroACS_Current_Sensor::Robojax_AllegroACS_Current_Sensor(int model, int vinPin, float vcc)
{
	// -- value
	_vin = vinPin;
	_model = model;
	_vcc = vcc;
}

/*
* getCurrent()
returns the current

 Written by Ahmad Shamshiri on Mar 14, 2020
 www.Robojax.com
 float readCurrent();
*
*/
float Robojax_AllegroACS_Current_Sensor::getCurrent()
{
	readCurrent();
	return this->_current;
} //getCurrent()

/*
 getCurrentAverage
 Get the average of current reading every i count
 i is number of iteration
Written by Ahmad Shamshiri on Mar 15, 2020 at 08:04 in Ajax, Ontario, Canada
*/
float Robojax_AllegroACS_Current_Sensor::getCurrentAverage(int i)
{
	float sum = 0, average;
	if (i > 1)
	{
		for (int c = 0; c < i; c++)
		{
			sum += getCurrent();
		}
	}
	else
	{
		Serial.println("getCurrentAverage(a), a must be greator than 0,\nexample getCurrentAverage(5)");
		exit(1);
	}
	average = sum / i;
	return average;
} //getCurrentAverage end

/*
* readCurrent()
reads the current and sets the _voltage and _current

 Written by Ahmad Shamshiri on Mar 14, 2020
 www.Robojax.com
 float readCurrent();
*
*/
float Robojax_AllegroACS_Current_Sensor::readCurrent()
{
	const float FACTOR = this->_sensitivity[this->_model] / 1000;				  // set sensitivity for selected model
	const float QOV = this->_quiescent_Output_voltage[this->_model] * this->_vcc; // set quiescent Output voltage for selected model
	float voltage;																  // internal variable for voltage
	float voltage_raw = (5.0 / 1023.0) * analogRead(this->_vin);				  // Read the voltage from sensor
	voltage = voltage_raw - QOV;
	this->_outVoltage = voltage;
	this->_current = abs(voltage / FACTOR) - (this->_magnetic_offset_error[this->_model] / 1000);

} //readCurrent()

void Robojax_AllegroACS_Current_Sensor::debug()
{
	Serial.println("Settings");
	Serial.print("\tVersin: ");
	Serial.print(this->_version);
	Serial.print(" Model:");
	Serial.print(this->_model);
	Serial.print(" ");
	Serial.print(_modelNames[this->_model]);
	Serial.print(" Sensitivity:");
	Serial.print(this->_sensitivity[this->_model]);
	Serial.print(" Quiesent voltage:");
	Serial.println(this->_quiescent_Output_voltage[this->_model]);

	Serial.println("Readings");
	Serial.print("\tInput Pin:");
	Serial.print(this->_vin);
	Serial.print("\tSense Voltage:");
	Serial.print(analogRead(this->_vin) * 5.0 / 1023.0, 3);

	Serial.print("V, Current:");
	Serial.print(this->getCurrent());
	Serial.println("A");

	Serial.print("\tAverage of 300 readings: ");
	Serial.print(getCurrentAverage(300));
	Serial.println("A");

	Serial.println("=====================");
} //readCurrent()
