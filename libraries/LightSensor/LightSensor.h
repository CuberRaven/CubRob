#ifndef LightSensor_h
#define LightSensor_h

#include "Arduino.h"
#include "stdint.h"

class LightSensor
{
private:
    uint8_t _pin;
public:
    LightSensor(uint8_t pin);
    LightSensor();
    uint16_t get_data();
    ~LightSensor();
};

#endif
