#include "Wire.h"
#include "SensorDevs.h"
#include "LightSensor.h"



/* 
 * Orange Pi и микроконтроллеры на плате могут обмениваться сообщениями друг с другом,
 * используя протокол I2C. Согласно этому протоколу, одно устройство является ведущим (Master),
 * а все остальные - ведомыми (Slave).
 *
 * Ведущий может запрашивать данные у ведомых, и отправлять им сообщения, в то же время ведомые
 * могут только отвечать ведущему, когда он их спросит.
 *
 * Чтобы было понятно, к какому именно устройству обращается ведущий, все ведомые имеют уникальный
 * адрес. 
 */

#define SENSOR_ADDR 10

int lightData = 0; // Показания датчика освещенности
LightSensor myLight(LIGHT_SENSOR);

byte slave_address = SENSOR_ADDR; // Адрес контроллера, его можно изменить на MOTOR_ADDR или SENSOR_ADDR
byte from_orangepi[3] = {0};

void onReceive(int len) {
  Serial.printf("%d байт от Master : ", len);
  int bytes_received = 0;
  while (Wire.available()) {
    from_orangepi[bytes_received] = Wire.read();
    Serial.printf("[%u]", from_orangepi[bytes_received]);
    bytes_received++;
  }
  byte red = from_orangepi[1];
  byte green = from_orangepi[2];
  byte blue = from_orangepi[3];

  led_set(red,green,blue);

}

/* Для обработки запроса от OrangePi */
void onRequest() {
  lightData = myLight.get_data();
  Wire.write(lightData);
  Serial.println("Ответ на запрос отправлен!");
}

void led_set(byte red, byte green, byte blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

void setup() {
  Serial.begin(115200);

  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin(slave_address);
}

void loop() {}
