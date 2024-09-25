
#include "esp32-hal-ledc.h"
#include <MotorDevs.h> 
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

long long int encoderData = 0;

int speed = 125;

void setup() {
  Serial.begin(115200);

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
 * Функция для движения робота. 
 * speed - скорость (0-255).
 * forward - направление. true - вперед, false - назад
 */
void move(byte speed, bool forward) {

  if (forward) {
    for (int i = 0; i < ENC_NUM; i++) {
      ledcWrite(motor_P_pins[i], speed);
      ledcWrite(motor_N_pins[i], 0);
    }
  } else {
    for (int i = 0; i < ENC_NUM; i++) {
      ledcWrite(motor_P_pins[i], 0);
      ledcWrite(motor_N_pins[i], speed);
    }
  }
  
}

void loop() {
  /* Заставляем мотор двигаться */
  move(speed,1);

  /* Получаем данные с энкодера */
  Serial.printf("Энкодеры: ");
  for (int i = 0; i < ENC_NUM; i++) {
    Serial.printf("|%lld\t", wheelEncoders[i].getCount());
  }
  Serial.println("|");
  delay(100);

}
