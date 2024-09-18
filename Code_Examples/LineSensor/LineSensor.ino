#include "SensorDevs.h"
#include "LineSensor.h"

LineSensor mySensor(LINE_PIN_0, LINE_PIN_1, LINE_PIN_2, LINE_PIN_3, LINE_PIN_4);
const int OUTPUTS = 5; // Датчик линии имеет 5 выходов, показывающих состояние линии
bool lineStates[OUTPUTS] = {0}; // Массив состояний датчика линии

void setup() {
  Serial.begin(115200);
}

void loop() {
  mySensor.get_data(lineStates);
  Serial.print("Состояние линии: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%u|", lineStates[i]); // Выведем значение с каждого выхода. "%u" используется для byte и bool значений
  }
  Serial.println(""); // Переведем строку
  delay(100);
}
