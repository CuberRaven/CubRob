#include <ServoDevs.h> 
#include <ServoSmooth.h>

/* Здесь показан пример с более плавным движением сервопривода */
ServoSmooth claw; // Объявим переменную сервопривода

/* 
 * Для самого схвата углы устанавливаются только в пределах 90-180 градусов 
 * 90   - Полностью открыт
 * 180  - Полностью закрыт
 */

// Углы для открытого и закрытого схвата
int clawOpen = 120;
int clawClose = 180;

int max_speed = 50;
int max_acceleration = 0.1;

bool open_claw = false; // Переменная, показывающая, закрывается клешня или открывается
long long int change_time = 0; // будем фиксировать время, когда мы начинаем открывать и закрывать схват

void setup() {
  Serial.begin(115200);

  /* 
   * Используем передний схват.
   * Вторым аргументом в функцию attach для любого из схватов 
   * НЕОБХОДИМО добавлять какой-либо начальный угол в пределах 90-180 градусов
   */
  claw.attach(PIN_CLAW_GRAB, clawOpen);
  claw.smoothStart(); // Плавно движемся в стартовую позицию (в нашем случае 135 градусов)

  claw.setSpeed(max_speed);    // Установим ограничение скорости
  claw.setAccel(max_acceleration);   	  // и ограничение ускорения

}


void loop() {
  claw.tick(); // Эта функция должна вызываться как можно чаще

  if (millis() - change_time >= 1000) { // каждую секунду
    change_time = millis();
    open_claw = !open_claw;
    if (open_claw) {
      Serial.println("Схват открывается ...");
      claw.setTargetDeg(clawOpen); // Зададим угол, в который мы хотим поехать
    } else {
      Serial.println("Схват закрывается ...");
      claw.setTargetDeg(clawClose);
    }
  }
  
}
