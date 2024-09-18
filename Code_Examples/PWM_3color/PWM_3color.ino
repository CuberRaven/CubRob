#include "SensorDevs.h"
#include "esp32-hal-ledc.h"

const int freq = 5000;        // Частота ШИМ (Hz)
const int resolution = 10;    // Разрешение (бит)

void setup() {
  /* Конфигурация ШИМ для одного канала светодиода */
  ledcAttach(LED_RED, freq, resolution);
  ledcAttach(LED_GREEN, freq, resolution);
  ledcAttach(LED_BLUE, freq, resolution);
}

void loop() {
  /* Плавное увеличение яркости */
  for (int i = 0; i <= 1023; i++) {
    ledcWrite(LED_RED, i);
    ledcWrite(LED_GREEN, i);
    ledcWrite(LED_BLUE, i);
    delay(1); // Задержка для плавности
  }
  
  /* Плавное уменьшение яркости */
  for (int i = 1023; i >= 0; i--) {
    ledcWrite(LED_RED, i);
    ledcWrite(LED_GREEN, i);
    ledcWrite(LED_BLUE, i);
    delay(1); // Задержка для плавности
  }
}
