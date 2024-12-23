/*
 * Этот скетч делает MOTOR_ESP ведомым I2C-устройством.
 * Контроллер двигает моторы со скоростями, которые получит от ведущего
 */

#include "esp32-hal-ledc.h"
#include <MotorDevs.h> 
#include <Wire.h> 
#include <ESP32Encoder.h> 

const int ENC_NUM = 4;

ESP32Encoder wheelEncoders[ENC_NUM];
const int PWM_FREQ       = 40000;
const int PWM_RESOLUTION = 8;

/* Пины A и B для энкодеров */
byte enc_A_pins[ENC_NUM] = {ENC1_A, ENC2_A, ENC3_A, ENC4_A};
byte enc_B_pins[ENC_NUM] = {ENC1_B, ENC2_B, ENC3_B, ENC4_B};

/* Пины + и - для моторов */
byte motor_P_pins[ENC_NUM] = {M1P, M2P, M3P, M4P};
byte motor_N_pins[ENC_NUM] = {M1N, M2N, M3N, M4N};

long long int encoderData[ENC_NUM] = {0};

int speed = 125;

/* Получение знакового целого числа из байт */
int16_t make_int16(uint8_t* buf) {
  return (buf[0] << 8) + buf[1];
}

// 9 байт приходит с OrangePi. Первый байт не используем
void onReceive(int len) {
  Serial.printf("От Master [%d]: ", len);
  byte from_master[len-1] = {0};

  int16_t speed[4] = {0};
  int bytes = 0;

  while (Wire.available()) {
    from_master[bytes] = Wire.read();
    Serial.printf("[%u]", from_master[bytes]);
    bytes++;
  }
  for (int i = 0; i < 4; i++) {
    speed[i] = make_int16(from_master+1 + i*2);
    Serial.printf("\nD: %d\n", speed[i]);
  }
  move_robot(speed[0], speed[1], speed[2], speed[3]);
}

void setup() {
  Serial.begin(115200);
  Wire.onReceive(onReceive);
  Wire.begin(11);

  for (int i = 0; i < ENC_NUM; i++) {
    /* Подключим энкодеры */
    wheelEncoders[i].attachFullQuad(enc_A_pins[i], enc_B_pins[i]);
    wheelEncoders[i].setCount(0);

    /* Подключим моторы */
    ledcAttach(motor_P_pins[i], PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(motor_N_pins[i], PWM_FREQ, PWM_RESOLUTION);
  }
}

/*
 * Функция для движения мотора. 
 * speed - скорость (0-255). Может быть отрицательной, знак определяет направление вращения
 * pin - номер пинов мотора
 */
void move_motor(byte pin, int speed) {
  Serial.printf("SPD: %d\n", speed);
  if (speed > 0) {
    ledcWrite(motor_N_pins[pin], speed);
    ledcWrite(motor_P_pins[pin], 0);
  } else {
    Serial.printf("else\n");
    ledcWrite(motor_N_pins[pin], 0);
    ledcWrite(motor_P_pins[pin], -speed);
  }
}

/*
 * Функция для движения робота. 
 * speed_X - скорость (0-255) колеса X. 
 * F - переднее, B - заднее
 * R - правое L - левое
 * (FR - переднее правое)
 */
void move_robot(int speed_FR, int speed_FL, int speed_BL, int speed_BR) {

  move_motor(0, speed_FR);
  move_motor(1, speed_FL);
  move_motor(2, speed_BL);
  move_motor(3, speed_BR);
}

void loop() {}
