#include "sensorsm.h"


SENSORSM::SENSORSM(PinName pin_num):pin(pin_num),c_state(true),o_state(true){}

SENSORSM::~SENSORSM(){}


void SENSORSM::update()
{
    this->o_state = this->c_state;
    this->c_state = this->pin.read();  
}

bool SENSORSM::change()
{
    return (this->c_state != this->o_state); 
}

bool SENSORSM::fflank()
{
    return (this->o_state & !this->c_state);
}





