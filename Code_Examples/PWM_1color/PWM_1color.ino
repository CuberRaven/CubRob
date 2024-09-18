#include "SensorDevs.h"
#include "esp32-hal-ledc.h"

const int ledPin = LED_RED;   // Вывод, к которому подключен светодиод
const int freq = 5000;        // Частота ШИМ (Hz)
const int resolution = 10;    // Разрешение (бит)

void setup() {
  /* Конфигурация ШИМ для одного канала светодиода */
  ledcAttach(LED_RED, freq, resolution);
}

void loop() {
  /* Плавное увеличение яркости */
  for (int i = 0; i <= 1023; i++) {
    ledcWrite(ledPin, i);
    delay(2); // Задержка для плавности
  }
  
  /* Плавное уменьшение яркости */
  for (int i = 1023; i >= 0; i--) {
    ledcWrite(ledPin, i);
    delay(2); // Задержка для плавности
  }
}
