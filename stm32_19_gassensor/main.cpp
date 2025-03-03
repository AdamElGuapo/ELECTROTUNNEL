/* Copyright (c) 2022 Perlatecnica APS ETS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/****************************************************
*            RAPID PROTOTYPING WITH NUCLEO          *
* Example Code 19: Gas Sensor			            *
* Author: Mauro D'Angelo                            *
* Organization: Perlatecnica APS ETS                *
*****************************************************/

#include "mbed.h"

// It creates an instance of the AnalogIn class.
// x_value and y_value are the name of the variables (instance).
// The instances shall 'point' to the pin A0 and A1
AnalogIn a0_value(A0);
DigitalIn d0_value(D4);

DigitalOut led(LED1);

int main() {
	float a0_meas;
	int d0_meas;

	while(1) {
		// It reads the analog input value (value from 0.0 to 1.0)
		a0_meas = a0_value.read();
		d0_meas = d0_value.read();

		// It prints the measure
		printf("Analog value = %f Digital value = %d\r\n", a0_meas, d0_meas);

		wait(0.2); // wait for 200 ms
	}
}

