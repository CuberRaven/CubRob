/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>
#include <ESP32Encoder.h> 
#include <SensorDevs.h>
#include <MotorDevs.h> 

/* Моторы */
const int ENC_NUM = 4;

ESP32Encoder wheelEncoders[ENC_NUM];
const int PWM_FREQ       = 40000;
const int PWM_RESOLUTION = 8;

/* Пины A и B для энкодеров */
byte enc_A_pins[ENC_NUM] = {ENC1_A, ENC2_A, ENC3_A, ENC4_A};
byte enc_B_pins[ENC_NUM] = {ENC1_B, ENC2_B, ENC3_B, ENC4_B};

/* Пины + и - для моторов */
byte motor_P_pins[ENC_NUM] = {M1P, M2P, M3P, M4P};
byte motor_N_pins[ENC_NUM] = {M1N, M2N, M3N, M4N};

long long int encoderData = 0;

int speed = 125;

/*
 * Параметры точки доступа WiFi.
 * Пароль должен содержать как минимум 7 символов.
 */
const char *ssid = "RobotAP";
const char *password = "12345678";

NetworkServer server(80);

void setup() {
  /* Инициализируем все моторы и энкодеры */
  for (int i = 0; i < ENC_NUM; i++) {
    /* Подключим энкодеры */
    wheelEncoders[i].attachFullQuad(enc_A_pins[i], enc_B_pins[i]);
    wheelEncoders[i].setCount(0);

    /* Подключим моторы */
    ledcAttach(motor_P_pins[i], PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(motor_N_pins[i], PWM_FREQ, PWM_RESOLUTION);
  }

  Serial.begin(115200);
  Serial.println();
  Serial.println("Создание точки доступа ...");

  WiFi.softAP(ssid, password);
  
  /* Адрес будет: 192.168.4.1 */
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP: ");
  Serial.println(myIP);
  server.begin();
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // Если есть новый клиент
    Serial.println("New Client.");  // сообщим о нем в последовательный порт
    String currentLine = "";        // Строка для данных от клиента

    while (client.connected()) {    // Пока клиент подключен
      if (client.available()) {     // Если доступны байты для чтения от клиента
        char c = client.read();     // Прочитать байт от клиента
        Serial.write(c);            // Вывести его в последовательный порт

        if (c == '\n') {            // Если получили символ перевода строки,

          /*
           * то проверим длину строки currentLine от клиента. Если она равна нулю, значит
           * мы получили два символа перевода строки подряд, а это - конец запроса от клиента.
           */
          if (currentLine.length() == 0) {
            /* Отправляем ответ клиенту */
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Содержимое ответа
            client.print("<button onclick=\"location.href='http://192.168.4.1/W'\">FORWARD</button><br>");
            client.print("<button onclick=\"location.href='http://192.168.4.1/S'\">BACK</button><br>");
            client.print("<button onclick=\"location.href='http://192.168.4.1/A'\">LEFT</button><br>");
            client.print("<button onclick=\"location.href='http://192.168.4.1/D'\">RIGHT</button><br>");
            client.print("<button onclick=\"location.href='http://192.168.4.1/N'\">STOP</button><br>");

            // HTTP-ответ заканчивается символом перевода строки
            client.println();
            // Прервем цикл
            break;
          } else {  // Если строка currentLine не пуста, очистим ее
            currentLine = "";
          }
        } else if (c != '\r') {  // добавим пришедший от клиента символ к строке
          currentLine += c;      
        }

        // Проверим запрос от клиента и заставим робот двигаться в зависимости от нажатой кнопки на веб-странице
        if (currentLine.endsWith("GET /W")) {
          move(speed, false);  
        }
        if (currentLine.endsWith("GET /S")) {
          move(speed, true);  
        }
        if (currentLine.endsWith("GET /A")) {
          turnLeft(speed);  
        }
        if (currentLine.endsWith("GET /D")) {
          turnRight(speed);  
        }
        if (currentLine.endsWith("GET /N")) {
          move(0, true);  
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

/* =============== Функции для движения колес =============== */
void moveWheel(byte wheel_num, byte spd, bool forward) {
  if (wheel_num > 3) {
    Serial.printf("Номер колеса %u за пределами допустимых значений (0 - 3)!\n", wheel_num);
    return;
  }
  if (forward) {
    ledcWrite(motor_P_pins[wheel_num], spd);
    ledcWrite(motor_N_pins[wheel_num], 0);
  } else {
    ledcWrite(motor_P_pins[wheel_num], 0);
    ledcWrite(motor_N_pins[wheel_num], spd);
  }
}

void move(byte speed, bool forward) {
  if (forward) {
    for (int i = 0; i < ENC_NUM; i++) {
      moveWheel(i, speed, true);
    }
  } else {
    for (int i = 0; i < ENC_NUM; i++) {
      moveWheel(i, speed, false);
    }
  }
}

void turnLeft(byte speed) {
  moveWheel(0, speed, false);
  moveWheel(3, speed, false);

  moveWheel(1, speed, true);
  moveWheel(2, speed, true);
}

void turnRight(byte speed) {
  moveWheel(1, speed, false);
  moveWheel(2, speed, false);

  moveWheel(0, speed, true);
  moveWheel(3, speed, true);
}
/* ============================================================ */
