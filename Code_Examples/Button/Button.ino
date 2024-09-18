#include "SensorDevs.h"

int buttonState = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_RED, OUTPUT);

}

void loop() {
  buttonState = digitalRead(BUTTON_PIN); // Прочитаем состояние кнопки

  // Проверим состояние кнопки: HIGH - кнопка нажата
  if (buttonState == HIGH) {
    digitalWrite(LED_RED, HIGH); // Включим светодиод
  } else {
    digitalWrite(LED_RED, LOW); // Выключим светодиод
  }

}
