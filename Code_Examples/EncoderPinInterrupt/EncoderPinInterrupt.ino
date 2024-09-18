#include "esp32-hal-ledc.h"
#include <MotorDevs.h> 

/* Счетчик тиков энкодера */
int encoder_pinA_data = 0;

/* 
 * Эта функция будет вызываться при прерывании 
 */
void IRAM_ATTR A_encPinInterrupt() {
  encoder_pinA_data++;
}

/* Зададим параметры ШИМ */
const int PWM_FREQ       = 40000;
const int PWM_RESOLUTION = 8;

int speed = 125;

void setup() {
  Serial.begin(115200);
  pinMode(ENC1_A, INPUT);
  ledcAttach(M1N, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(M1P, PWM_FREQ, PWM_RESOLUTION);

  /*
   * Добавляем прерывание, указав пин энкодера, функцию, которая вызовется при преывании, а также режим прерывания
   * Режимов три: RISING - переход от высокого уровня к низкому
   *              FALLING - от низкого к высокому
   *              CHANGE - по любому из двух событий выше.
   */
  attachInterrupt(digitalPinToInterrupt(ENC1_A), A_encPinInterrupt, CHANGE); 

}

void loop() {
  /* Заставляем мотор двигаться */
  ledcWrite(M1N, 0);
  ledcWrite(M1P, speed);

  /* Получаем данные с энкодера */
  Serial.printf("Энкодер: ");
  Serial.println(encoder_pinA_data);
  delay(100);

}