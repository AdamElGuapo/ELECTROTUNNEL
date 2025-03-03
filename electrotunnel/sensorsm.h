#ifndef SENSORSM_H
#define SENSORSM_H

#include "mbed.h"
#include "platform/mbed_thread.h"
#include <cstdint>

class SENSORSM
{
    private:
    DigitalIn pin;
    bool c_state;
    bool o_state;


    public:
        SENSORSM(PinName pin);
        ~SENSORSM();

    void update();
    bool change();

    bool fflank();



};

#endif