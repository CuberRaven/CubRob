#ifndef LineSensor_h
#define LineSensor_h

#include "Arduino.h"
#include "stdint.h"

const uint8_t LED_CNT = 5;

/*
    Output: UINT8_t [0 0 0 D1 D2 D3 D4 D5]
    D<x> - состояние светодиода <x>
*/
class LineSensor
{
private:
    uint8_t _THRESHOLD;
    uint8_t sensor_data[LED_CNT] = {0};
    bool line_status[LED_CNT] = {0};

    bool line_detected();
    bool is_black(uint8_t data);
    void initLineSensor();
    inline void set_bit(uint8_t &dst, bool val, uint8_t  bit);
public:
    uint8_t OUT[LED_CNT];
    uint8_t state;

    LineSensor(uint8_t out_1, uint8_t out_2, uint8_t out_3, uint8_t out_4, uint8_t out_5);
    LineSensor();
    uint8_t get_raw_data();
    void get_data(bool * input);

    ~LineSensor();
};

#endif