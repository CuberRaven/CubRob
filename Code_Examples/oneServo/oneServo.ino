#include <ServoDevs.h> 
#include <ESP32Servo.h>

Servo claw; // Объявим переменную сервопривода

/* 
 * Для самого схвата углы устанавливаются только в пределах 90-180 градусов 
 * 90   - Полностью открыт
 * 180  - Полностью закрыт
 */
int clawOpen = 135;
int clawClose = 180;

void setup() {
  Serial.begin(115200);
  claw.attach(PIN_CLAW_GRAB); // Будем использовать передний схват

}

void loop() {
  Serial.println("Схват открывается ...");
  claw.write(clawOpen);
  delay(1000);

  Serial.println("Схват закрывается ...");
  claw.write(clawClose);
  delay(1000);
}
