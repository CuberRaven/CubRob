#include <BluetoothSerial.h>
#include <ESP32Encoder.h> 
#include <MotorDevs.h> 

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
BluetoothSerial ESP_BT; // Объект Bluetooth

int incoming;

void setup() {

  Serial.begin(115200); // Запускаем последовательный монитор со скоростью 9600
  for (int i = 0; i < ENC_NUM; i++) {
    /* Подключим энкодеры */
    wheelEncoders[i].attachFullQuad(enc_A_pins[i], enc_B_pins[i]);
    wheelEncoders[i].setCount(0);

    /* Подключим моторы */
    ledcAttach(motor_P_pins[i], PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(motor_N_pins[i], PWM_FREQ, PWM_RESOLUTION);
  }

  ESP_BT.begin("ESP32_LED_Control"); // Имя хоста Bluetooth
  Serial.println("Bluetooth Device is Ready to Pair");
}

/* Двигаем колесо с номером 0-3 */
void moveWheel(byte wheel_num, bool direction) {

  if (wheel_num > 3) {
    Serial.printf("Номер колеса %u за пределами допустимых значений (0 - 3)!\n", wheel_num);
    return;
  }
  
  if (direction == false) {
    ledcWrite(motor_P_pins[wheel_num], 0);
    ledcWrite(motor_N_pins[wheel_num], speed);
  } else {
    ledcWrite(motor_P_pins[wheel_num], speed);
    ledcWrite(motor_N_pins[wheel_num], 0);
  }
}

void move(byte speed, bool direction) {

  if (direction == false) {
    for (int i = 0; i < ENC_NUM; i++) {
      ledcWrite(motor_P_pins[i], 0);
      ledcWrite(motor_N_pins[i], speed);
    }
  } else {
    for (int i = 0; i < ENC_NUM; i++) {
      ledcWrite(motor_P_pins[i], speed);
      ledcWrite(motor_N_pins[i], 0);
    }
  }
  
}

void loop() {
  if (ESP_BT.available()) // Проверяем, не получили ли мы что-либо по Bluetooth
  {
    incoming = ESP_BT.read(); // Считываем данные
    Serial.print("Received:"); Serial.println(incoming);
    if (incoming == 70)
    {
      move(speed, true);
      Serial.println("FWD");
    }
    if (incoming == 66)
    {
      move(speed, false);
      Serial.println("BWD");
    }
    if (incoming == 48)
    {
      move(0, false);
      Serial.println("LED turned OFF");
    }
  }
  delay(20);

}