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
#define RATE_SENSOR 2000
#include "mbed.h"
#include <chrono>

// Sensores
AnalogIn a0_value(A0);
DigitalIn d0_value(D4);
DigitalOut led(LED1);
DigitalIn Boton(PC_13);
AnalogIn a1_value(A1);

int main() {
    float a0_meas;
    int d0_meas;
    float voltage;
    bool led;
    int d0_boton;
    float a1_meas;
    while (1) {
        // Leer valores del sensor
        d0_boton = Boton.read();
        a0_meas = a0_value.read();
        voltage = a0_meas * 5;  // Si la referencia es 5V
        d0_meas = d0_value.read();

        a1_meas = a1_value.read();

        if (d0_boton==0){
            led=!led;
        }
        
        
        
        // Mostrar información más clara
        printf("Gas Sensor Readings:\n");
        printf("  Analog Value: %f -> Voltage: %.2fV\n", a0_meas, voltage);
        printf("  Digital Value: %d -> %s\n", d0_meas, d0_meas ? "No gas detected" : "Gas detected!");
        printf("Water Sensor Readings:\n");
        printf("  Analog Value: %f\n",a1_meas);
        printf("----------------------\n");
        

        ThisThread::sleep_for(RATE_SENSOR);
    }
}

