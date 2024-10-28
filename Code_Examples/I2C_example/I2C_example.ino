#include "Wire.h"

/* 
 * Микроконтроллеры на плате могут обмениваться сообщениями друг с другом,
 * используя протокол I2C. Согласно этому протоколу, одно устройство является ведущим (Master),
 * а все остальные - ведомыми (Slave).
 *
 * Ведущий может запрашивать данные у ведомых, и отправлять им сообщения, в то же время ведомые
 * могут только отвечать ведущему, когда он их спросит.
 *
 * Чтобы было понятно, к какому именно устройству обращается ведущий, все ведомые имеют уникальный
 * адрес. 
 */

const byte SLAVE_ADDR = 10;  // Адрес ведомого. Он устанавливается в скетче I2C_slaveSimple
int message_num = 0;

/* Сделаем отдельную функцию, которая будет отправлять сообщение на другой контроллер с помощью I2C */
void sendMessage() {
  Wire.beginTransmission(SLAVE_ADDR);  // Начинаем отправку сообщения ведомому контроллеру
  Wire.printf("Сообщение #%d\n", message_num); // Пишем сообщение
  Wire.endTransmission(); // Заканчиваем отправку
  message_num++;
}

void sendRequest() {
  uint8_t bytesReceived = Wire.requestFrom(SLAVE_ADDR, 16);
  Serial.printf("requestFrom: %u\n", bytesReceived);
  
  if (bytesReceived > 0) { 
    char from_slave[bytesReceived];
    Wire.readBytes(from_slave, bytesReceived);
    Serial.println(from_slave);
  } 
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  delay(1000);
  sendRequest();
  delay(1000);
  sendMessage();
}
