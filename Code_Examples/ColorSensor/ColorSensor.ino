#include <Wire.h>
#include "Adafruit_TCS34725.h"

/*
 * Параметр TCS34725_INTEGRATIONTIME_50MS можно изменить, уменьшая или увеличивая время интегрирования
 * Чем меньше время интегрирования, тем чаще опрашивается датчик, но менее точны становятся его показания
 * Все доступные параметры находятся в "Adafruit_TCS34725.h"
 */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(115200);

  if (!tcs.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Если датчик не найден, программа зависнет здесь
  }
}

void loop() {
  float red, green, blue;
  delay(60);

  tcs.getRGB(&red, &green, &blue);

  /* Выведем все составляющие цвета, представив их как целые числа */
  Serial.printf("R: %d\t G: %d\t B: %d\n", (int)red, (int)green, (int)blue);

}
