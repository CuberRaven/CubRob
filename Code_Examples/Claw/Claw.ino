#include <ServoDevs.h> 
#include <ESP32Servo.h>

/* Робот имеет 9 сервоприводов */
const uint8_t SERVO_CNT = 9;
Servo servos[SERVO_CNT];
uint8_t servo_pin[9] = {PIN_MANIP_0, PIN_MANIP_1, PIN_MANIP_2, PIN_MANIP_3,
    PIN_MANIP_4, PIN_CAM_HOR, PIN_CAM_VER, PIN_CLAW_GRAB, PIN_CLAW_VER};

/* Углы, в которые должны встать сервоприводы */
int clawUp = 0;
int clawDown = 90;

/* Для самого схвата углы устанавливаются только в пределах 90-180 градусов */
int clawOpen = 135;
int clawClose = 180;

void setup() {
  Serial.begin(115200);
  for (size_t i = 0; i < SERVO_CNT; i++) {
    servos[i].attach(servo_pin[i]);
  }
  Serial.println("Старт!");
}

void loop() {
  
  servos[SERVO_CLAW_VER].write(clawDown);
  delay(1000);
  servos[SERVO_CLAW_GRAB].write(clawOpen);
  delay(1000);
  servos[SERVO_CLAW_GRAB].write(clawClose);
  delay(1000);
  servos[SERVO_CLAW_VER].write(clawUp);
  delay(5000);
}
