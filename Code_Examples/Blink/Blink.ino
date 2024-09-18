#include "SensorDevs.h"

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  /* Включим по очереди красный, зеленый и синий каналы светодиода */
  digitalWrite(LED_RED, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);

  /* Так же поочередно выключим их */
  digitalWrite(LED_RED, LOW);
  delay(1000); 
  digitalWrite(LED_GREEN, LOW);
  delay(1000); 
  digitalWrite(LED_BLUE, LOW);
  delay(1000); 
}
