
#include "esp32-hal-ledc.h"
#include <MotorDevs.h> 
#include <ESP32Encoder.h> 

ESP32Encoder wheelEncoder;
const int PWM_FREQ       = 40000;
const int PWM_RESOLUTION = 8;

long long int encoderData = 0;

int speed = 125;

void setup() {
  Serial.begin(115200);
  wheelEncoder.attachFullQuad(ENC1_A, ENC1_B);
  wheelEncoder.setCount(0);

  ledcAttach(M1N, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(M1P, PWM_FREQ, PWM_RESOLUTION);

}

void loop() {
  /* Заставляем мотор двигаться */
  ledcWrite(M1N, 0);
  ledcWrite(M1P, speed);

  /* Получаем данные с энкодера */
  Serial.printf("Энкодер: ");
  encoderData = wheelEncoder.getCount();
  Serial.println(encoderData);
  delay(100);

}
