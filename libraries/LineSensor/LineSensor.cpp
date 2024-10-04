#include "LineSensor.h"
#include "Arduino.h"

#define LN_DEBUG false

LineSensor::LineSensor (uint8_t out_1, uint8_t out_2, uint8_t out_3, uint8_t out_4, uint8_t out_5) {
    OUT[0] = out_1; OUT[1] = out_2; OUT[2] = out_3; OUT[3] = out_4; OUT[4] = out_5;
    _THRESHOLD = 60;
    state = 0;
    initLineSensor();
    Serial.print("INIT LINE SENSOR");

}

LineSensor::LineSensor () {}

LineSensor::~LineSensor() {}

void LineSensor::initLineSensor() {
  for(int i = 0; i < LED_CNT; i++) {
    pinMode(OUT[i], INPUT);
  }
}

bool LineSensor::line_detected() {
  for (int i = 0; i < LED_CNT; i++) {
    if (line_status[i]) return true;
  }
  return false;
}

bool LineSensor::is_black(uint8_t data) {
  return data < _THRESHOLD;
}

inline void LineSensor::set_bit(uint8_t &dst, bool val, uint8_t  bit) {
  val ? dst |= (1 << bit) : dst &= ~(1 << bit);
}

/*
  Возвращает 1 байт, содержащий информацию о состоянии всех пяти выходов датчика линии

  Формат: [0 0 0 D5 D4 D3 D2 D1],
  где D<x> - состояние пина OUT<x>

  Пример: 0x19 - [0 0 0 1 1 0 0 1] - Линия обнаружена на OUT2 и OUT3
*/
uint8_t LineSensor::get_raw_data() {
  state = 0;
  if (LN_DEBUG) Serial.printf("Line sensor Data: \n");
  for(int i = 0; i < LED_CNT; i++) {
    
    sensor_data[i] = analogRead(OUT[i]);
    if (LN_DEBUG) Serial.printf("\nsensor_data: %d ", sensor_data[i]);
    line_status[i] = is_black(sensor_data[i]);
    if (LN_DEBUG) is_black(sensor_data[i]) ? Serial.print("1|") : Serial.print("0|"); 
    set_bit(state, line_status[i], i);
  }
  if (LN_DEBUG) Serial.println("");
  return state;
}

void LineSensor::get_data(bool * input) {
  for(int i = 0; i < LED_CNT; i++) {
    input[i] = is_black(analogRead(OUT[i]));
  }
}
