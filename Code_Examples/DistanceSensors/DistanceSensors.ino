#include <HCSR04.h>
#include "SensorDevs.h"

/* Инициализируем датчики растояния */
UltraSonicDistanceSensor Range_1(RANGE_1_TRIG,RANGE_1_ECHO);
UltraSonicDistanceSensor Range_2(RANGE_2_TRIG,RANGE_2_ECHO);
UltraSonicDistanceSensor Range_3(RANGE_3_TRIG,RANGE_3_ECHO);

void setup () {
  Serial.begin(115200);  
}

void loop () {
  /* Измерим расстояния на всех трёх датчиках */
  float dist_1 = Range_1.measureDistanceCm();
  float dist_2 = Range_2.measureDistanceCm();
  float dist_3 = Range_3.measureDistanceCm();

  /* Выведем расстояния в терминал */
  Serial.printf("Расстояния: %f \t %f \t %f \n", dist_1, dist_2, dist_3);
  delay(100);
}