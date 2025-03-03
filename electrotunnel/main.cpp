/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "platform/mbed_thread.h"

// Custom includes.
#include "sensorsm.h"
#include <cstdint>
#include "TextLCD.h"

// Blinking rate in milliseconds
#define BLINKING_RATE_MS    500
#define SAMP_RATE           10

#define SLAVEADDRESS 0x4E 
//#define SLAVEADDRESS 0x7E // Other possible Address

// It creates an instance of the i2c protocol driver assigning the pin SDA and SCL
I2C i2c_lcd(PB_9, PB_8); // SDA, SCL

// It creates the instance of the LCD driver. It needs the reference to I2C driver
TextLCD_I2C lcd(&i2c_lcd,SLAVEADDRESS,  TextLCD::LCD16x2); // I2C exp: I2C bus, PCF8574AT Slaveaddress, LCD Type

void carCounterf1();
void carCounterf2();
void semaforThread1();
void semaforThread2();
void ligth();
void updateScreen();
void gasUpdater();
void waterUpdater();

volatile int32_t car_count1 = 0;
volatile int32_t car_count2 = 0;
int32_t mid_lim  = 3;
int32_t high_lim = 5;

volatile bool gas_lim = false;

int main()
{
    // Initialise the digital pin LED1 as an output
    Thread ccounter1;
    ccounter1.start(carCounterf1);
    Thread ccounter2;
    ccounter2.start(carCounterf2);
    Thread semafor1;
    semafor1.start(semaforThread1);
    Thread semafor2;
    semafor2.start(semaforThread2);
    Thread ligths;
    ligths.start(ligth);
    Thread gas;
    gas.start(gasUpdater);
    Thread water;
    water.start(waterUpdater);

    Serial pc (USBTX, USBRX);
    pc.baud(115200);

    lcd.setCursor(TextLCD::CurOff_BlkOff);  // It switches off the cursor
    lcd.setBacklight(TextLCD::LightOn);     // It switches On the backlight
    lcd.cls();                              // It clens the screen
    lcd.printf("   Welcome to");            // It prints on the screen
    lcd.setAddress(0, 1);                   // It points to the first column of the second raw
    lcd.printf(" Electrunnel ");         // It prints on the screen
    
    ThisThread::sleep_for(BLINKING_RATE_MS);

    Thread screen;
    screen.start(updateScreen);

    pc.printf("Serial inicialized.\n");
    while (true) 
    {
        pc.printf("car count %d, %d, %d\n", car_count1, car_count2, gas_lim);
        ThisThread::sleep_for(BLINKING_RATE_MS);
    }
}

void carCounterf1()
{
    SENSORSM s1(PC_8);
    SENSORSM s2(PC_6);

    while (true) 
    {
        s1.update();
        s2.update();
        
        if(s1.fflank() | s2.fflank())
        {
            car_count1 += s1.fflank() - s2.fflank();
            car_count1 = (car_count1 < 0 )? 0 : car_count1;
        }
        ThisThread::sleep_for(SAMP_RATE);
    }
}

void carCounterf2()
{
    SENSORSM s3(D13);
    SENSORSM s4(D12);

    while (true) 
    {
        s3.update();
        s4.update();
        
        if(s3.fflank() | s4.fflank())
        {
            car_count2 += s3.fflank() - s4.fflank();
            car_count2 = (car_count2 < 0 )? 0 : car_count2;
        }
        ThisThread::sleep_for(SAMP_RATE);
    }
}

void semaforThread1()
{
    DigitalOut semafor[3]= {D8, D7, D4};
    uint8_t out = 0x00;

    while (true) 
    {
        if (car_count1 > high_lim) {out = (1<<2);}
        else if (car_count1 > mid_lim){out = (1<<1);}
        else {out = 0x01;}

        // if (gas_lim) {out = (1<<2);}

        for (uint8_t i = 0; i < 3; i++) 
        {
            semafor[i].write((out>>i) & 0x01);
        }        
        ThisThread::sleep_for(SAMP_RATE);
    }
}

void semaforThread2()
{
    DigitalOut semafor[3]= {D9, D10, D11};
    uint8_t out = 0x00;

    while (true) 
    {
        if (car_count2 > high_lim) {out = (1<<2);}
        else if (car_count2 > mid_lim){out = (1<<1);}
        else {out = 0x01;}

        //if (gas_lim) {out = (1<<2);}

        for (uint8_t i = 0; i < 3; i++) 
        {
            semafor[i].write((out>>i) & 0x01);
        }        
        ThisThread::sleep_for(SAMP_RATE);
    }
}

void ligth()
{
    PwmOut i_lights[3]= {D5, D3, D6};
    DigitalIn l_sensor(D2);

    float day[3] = {0.0f, 0.25f, 0.35f};
    float night[3] = {0.50f, 0.75f, 1.0f};

    for (uint8_t i = 0; i < 3; i++) 
    {
        i_lights[i].period_us(10);
    }

    while (true) 
    {
        if (car_count1 > 0 | car_count2 > 0) 
        {
            if (!l_sensor.read()) 
            {
                for (uint8_t i = 0; i < 3; i++) 
                {
                    i_lights[i].write(day[i]);
                }
            }
            else 
            {
                for (uint8_t i = 0; i < 3; i++) 
                {
                    i_lights[i].write(night[i]);
                }
            }
        }
        else 
        {
            for (uint8_t i = 0; i < 3; i++) 
                {
                    i_lights[i].write(0);
                }
        }

        ThisThread::sleep_for(100);
    }
}

void updateScreen()
{
    lcd.cls();

    while(true)
    {
        lcd.setAddress(0, 0);   
        lcd.printf("Car lane 1: %d", car_count1);           // It prints on the screen
        if (car_count1 < 10) 
        {
            lcd.setAddress(13, 0);
            lcd.printf("  ");
        }
        lcd.setAddress(0, 1);                               // It points to the first column of the second raw
        lcd.printf("Car lane 2: %d", car_count2);           // It prints on the screen
        if (car_count2 < 10) 
        {
            lcd.setAddress(13, 1);
            lcd.printf("  ");
        }
        ThisThread::sleep_for(100);
    }
}

void gasUpdater()
{
    DigitalIn gas_sensor(PC_5);
    while (true) 
    {
        gas_lim = gas_sensor.read();
        ThisThread::sleep_for(SAMP_RATE);
    }
}

void waterUpdater()
{
    AnalogIn water_sensor(A0);
    DigitalOut motor(PA_12);

    while (true) 
    {
        motor.write(water_sensor.read() <= 0.5);
        ThisThread::sleep_for(SAMP_RATE);
    }
}

