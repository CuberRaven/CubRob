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

#define SENSOR_ADDR 10
#define MOTOR_ADDR 11

byte slave_address = SENSOR_ADDR; // Адрес контроллера, его можно изменить на MOTOR_ADDR или SENSOR_ADDR

char from_sensor[16] = "Answer from SNR"; // Ответ от Sensor, он отправляется на ведущий контроллер и выводится там
char from_motor[16] = "Answer from MTR";  // Ответ от Motor, он отправляется на ведущий контроллер и выводится там


/* Функция для обработки запросов ведомым. Она не должна иметь аргументов */
void onRequest() {
  if (slave_address == SENSOR_ADDR) {
    Wire.print(from_sensor);
    Wire.print('\0'); // Нуль-символ, который обозначает конец строки
  }
  if (slave_address == MOTOR_ADDR) {
    Wire.print(from_motor);
    Wire.print('\0'); // Нуль-символ, который обозначает конец строки
  }
  
  Serial.println("Ответ на запрос отправлен!");
}

void onReceive(int len) {
  Serial.printf("От Master [%d]: ", len);
  while (Wire.available()) {
    Serial.write(Wire.read());
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin(slave_address);
}

void loop() {}
