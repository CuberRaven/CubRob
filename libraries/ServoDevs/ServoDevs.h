#ifndef SERVO_DEVS 
#define SERVO_DEVS

#include <Arduino.h>

/* Пины всех сервоприводов. для servo.attach(PIN_<X>) */
const uint8_t PIN_MANIP_0 	= 16;
const uint8_t PIN_MANIP_1 	= 17;
const uint8_t PIN_MANIP_2     = 12;
const uint8_t PIN_MANIP_3 	= 13;
const uint8_t PIN_MANIP_4 	= 27;
const uint8_t PIN_CAM_HOR 	= 26;
const uint8_t PIN_CAM_VER 	= 25;
const uint8_t PIN_CLAW_GRAB 	= 33;
const uint8_t PIN_CLAW_VER 	= 32;

/* Номера всех сервоприводов. для доступа к элементам массива Servo. */
const uint8_t SERVO_MANIP_0 	= 0;
const uint8_t SERVO_MANIP_1 	= 1;
const uint8_t SERVO_MANIP_2 	= 2;
const uint8_t SERVO_MANIP_3 	= 3;
const uint8_t SERVO_MANIP_4 	= 4;
const uint8_t SERVO_CAM_HOR 	= 5;
const uint8_t SERVO_CAM_VER 	= 6;
const uint8_t SERVO_CLAW_GRAB 	= 7;
const uint8_t SERVO_CLAW_VER 	= 8;

#endif
