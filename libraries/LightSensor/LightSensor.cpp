#include "LightSensor.h"

LightSensor::LightSensor(uint8_t pin) : _pin(pin){
    pinMode(pin, INPUT);
}

LightSensor::LightSensor() {}

uint16_t LightSensor::get_data() {
    return analogRead(_pin);
}

LightSensor::~LightSensor(){}