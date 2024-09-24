#include "SensorDevs.h"

// Объявим две функции для каждого ядра контроллера
void TaskBlinkCode(void *pvParameters);
void TaskButtonCode(void *pvParameters);

TaskHandle_t TaskButtonRead;
TaskHandle_t TaskBlinkLED;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

  uint32_t blink_delay = 1000;

  // Создадим две задачи, которые будут работать независимо друг от друга:
  // Для первого ядра:
  xTaskCreatePinnedToCore(
    TaskBlinkCode, 
    "LED blink", 
    2048, // Размер стека
    NULL,  
    1, // Приоритет данной задачи
    &TaskBlinkLED,  // TaskHandle, определенный в начале
    0  // 0 - первое ядро
  );

  // Для второго ядра:
  xTaskCreatePinnedToCore(
    TaskButtonCode, 
    "Button Read", 
    2048, // Размер стека
    NULL,  
    2, // Приоритет данной задачи
    &TaskButtonRead,  // TaskHandle, определенный в начале
    1  // 1 - второе ядро
  );

}

void loop() {
  if (TaskButtonRead != NULL) {  // Убедимся, что задача существует
    delay(5000);
    vTaskDelete(TaskButtonRead);  // Удалим ее, если она не определена
    TaskButtonRead = NULL;        // Предотвратим вызов vTaskDelete для несуществующей задачи
  }
  if (TaskBlinkLED != NULL) {  // Убедимся, что задача существует
    delay(5000);
    vTaskDelete(TaskBlinkLED);  // Удалим ее, если она не определена
    TaskBlinkLED = NULL;        // Предотвратим вызов vTaskDelete для несуществующей задачи
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlinkCode(void *pvParameters) {  // Код задачи
  /*
   * Мигание светодиодом
   * Включает зеленый канал светодиода на секунду, затем выключает на секунду
   *  
   * Функция delay() используется для ожидания перед тем как включить или выключить светодиод
   * Эта функция полностью блокирует ядро и не дает ему "заниматься другими делами".
   */

  while(true) { // Этот участок кода будет выполняться бесконечно
    digitalWrite(LED_GREEN, HIGH); // Включим зеленый цвет
    delay(blink_delay);            // Ждем секунду
    digitalWrite(LED_GREEN, LOW);  // Выключим зеленый канал светодиода
    delay(blink_delay);            // Подождем снова, перед тем как включить
  }
}

void TaskButtonCode(void *pvParameters) {  // Код задачи
  /*
   * Так как контроллер ESP32 имеет два ядра, мы можем запустить еще один бесконечный цикл,
   * который работает независимо от первого. В нем мы будем опрашивать состояние кнопки
   * и зажигать красный канал светодиода, если кнопка нажата.
   */

  while(true) { // Этот участок кода будет выполняться бесконечно, но на другом ядре
    int buttonState = digitalRead(BUTTON_PIN); // Прочитаем состояние кнопки
    // Проверим состояние кнопки: HIGH - кнопка нажата
    if (buttonState == HIGH) {
      digitalWrite(LED_RED, HIGH); // Включим светодиод
    } else {
      digitalWrite(LED_RED, LOW); // Выключим светодиод
    }
  }
}
