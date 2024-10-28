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

const byte SENSOR_ADDR = 10;  // Адрес Sensor ESP
const byte MOTOR_ADDR = 11;  // Адрес Motor ESP
int sensor_msg_num = 0;
int motor_msg_num = 0;

/* Сделаем отдельную функцию, которая будет отправлять сообщение на другой контроллер с помощью I2C */
void sendMessage(byte address) {
  Wire.beginTransmission(address);  // Начинаем отправку сообщения ведомому контроллеру
  if (address == SENSOR_ADDR) {
    Wire.printf("Сообщение для SENSOR ESP:#%d\n", sensor_msg_num); // Пишем сообщение
    sensor_msg_num++;
  }
  if (address == MOTOR_ADDR) {
    Wire.printf("Сообщение #%d\n", motor_msg_num); // Пишем сообщение
    motor_msg_num++;
  }
  Wire.endTransmission(); // Заканчиваем отправку
}

void sendRequest(byte addr) {
  uint8_t bytesReceived = Wire.requestFrom(addr, 16);
  Serial.printf("Запрос к ESP: %u\n", addr);
  
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
  sendRequest(SENSOR_ADDR);
  delay(250);
  sendRequest(MOTOR_ADDR);
  delay(1000);
  sendMessage(SENSOR_ADDR);
  sendMessage(MOTOR_ADDR);
  Serial.println("");
}
