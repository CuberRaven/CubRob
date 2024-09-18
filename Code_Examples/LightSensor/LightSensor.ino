#include "SensorDevs.h"
#include "LightSensor.h"

int lightData = 0; // Показания датчика освещенности
LightSensor myLight(LIGHT_SENSOR);

void setup() {
  Serial.begin(115200);
}

void loop() {
  /* Получим данные с датчика освещенности и выведем их на экран */
  lightData = myLight.get_data();
  Serial.print("Освещенность: ");
  Serial.println(lightData);

  delay(100);
}
