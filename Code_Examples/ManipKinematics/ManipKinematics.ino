#include <ServoDevs.h> 
// #include <ESP32Servo.h>
#include <ServoSmooth.h>

/* Здесь показан пример с более плавным движением сервопривода */
ServoSmooth base; // Объявим переменную сервопривода
ServoSmooth shoulder; // Объявим переменную сервопривода
ServoSmooth elbow; // Объявим переменную сервопривода
ServoSmooth pitch; // Объявим переменную сервопривода
#define DEBUG 1

/* Длины звеньев в мм */
uint32_t l1 = 87;
uint32_t l2 = 105;
uint32_t l3 = 76; 
uint32_t l4 = 115; 

int h1 = 120; // wrist
int h2 = 60;  // elbow
int h3 = 30; // shoulder
int h4 = 30; // base

int max_speed = 100;
int max_acceleration = 0.1;

inline double sqr(double x) {
  return x*x;
}

inline double degToRad(double x) {
  return (double)(x/180)*PI;
}

inline double radToDeg(double x) {
  return (double)(x/PI)*180;
}

inline double calcR(int th1, int th2, int th3) {
  double r_th1 = degToRad(th1);
  double r_th2 = degToRad(th2);
  double r_th3 = degToRad(th3);
  // Serial.printf("R of L1: %f\n",(double)l1*cos(r_th1));
  // Serial.printf("R of L2: %f\n",(double)l2*cos((PI/2)+r_th1-r_th2));
  // Serial.printf("R of L3: %f\n",(double)l3*cos(PI+r_th1-r_th3-r_th2));
  return ((double)l2*cos(r_th1) + (double)l3*cos((PI/2)+r_th1-r_th2) + (double)l4*cos(PI+r_th1-r_th3-r_th2));
}

inline double calcH(int th1, int th2, int th3) {
  double r_th1 = degToRad(th1);
  double r_th2 = degToRad(th2);
  double r_th3 = degToRad(th3);
  // Serial.printf("H of L1: %f\n",(double)l1*sin(r_th1));
  // Serial.printf("H of L2: %f\n",(double)l2*sin(r_th2+r_th1));
  // Serial.printf("H of L3: %f\n",(double)l3*sin(r_th3+r_th2));
  return (double)(l1 + l2*sin(r_th1) + l3*sin((PI/2)+r_th1-r_th2) + l4*sin(PI+r_th1-r_th3-r_th2));
}

void for_kin(int th0, int th1, int th2, int th3) {
  double r_th0 = degToRad(th0);
  double R = calcR(th1, th2, th3);
  double H = calcH(th1, th2, th3);

  double x = R*cos(r_th0);
  double y = R*sin(r_th0);
  double z = H;
  Serial.printf("FK: x = %f mm, y = %f mm, z = %f mm\n", x, y, z);
}

void inv_kin(double x, double y, double z, double phi) {
  phi = degToRad(phi);
  double th1 = acos(x/sqrt(sqr(x) + sqr(y)));
  Serial.printf("\nTH1: %f\n", radToDeg(th1));

  double x_ = x - l4*cos(phi);
  double y_ = y - l4*sin(phi);
  double XY = sqr(x_) + sqr(y_);
  double th3 = asin((XY - sqr(l2) - sqr(l3))/(2*l2*l3));
  Serial.printf("TH3: %f\n", radToDeg(th3));

  double beta = atan2(y_, x_);
  double alpha = acos((XY + sqr(l2) - sqr(l3))/(2*l2*sqrt(XY)));
  double th2_1 = beta + alpha;
  double th2_2 = beta - alpha;
  Serial.printf("TH2: %f or %f\n", radToDeg(th2_1), radToDeg(th2_2));

  // gamma = th3 - th2
  double gamma_1 = th3 - th2_1;
  double gamma_2 = th3 - th2_2;
  double th4_1 = PI - gamma_1 - phi;
  double th4_2 = PI + gamma_1 - phi;
  double th4_3 = PI - gamma_2 - phi;
  double th4_4 = PI + gamma_2 - phi;
  Serial.printf("TH4: %f or2 %f or3 %f or4 %f\n", radToDeg(th4_1), radToDeg(th4_2), radToDeg(th4_3), radToDeg(th4_4));
}

void setup() {
  Serial.begin(115200);

  pitch.attach(PIN_MANIP_0, h1);
  elbow.attach(PIN_MANIP_2, h2);
  shoulder.attach(PIN_MANIP_3, h3);
  base.attach(PIN_MANIP_4, h4);
  pitch.smoothStart();
  elbow.smoothStart();
  shoulder.smoothStart();
  base.smoothStart(); // Плавно движемся в стартовую позицию (в нашем случае 135 градусов)

  pitch.setSpeed(max_speed);    // Установим ограничение скорости
  pitch.setAccel(max_acceleration);   	  // и ограничение ускорения

  elbow.setSpeed(max_speed);    // Установим ограничение скорости
  elbow.setAccel(max_acceleration);   	  // и ограничение ускорения

  shoulder.setSpeed(max_speed);    // Установим ограничение скорости
  shoulder.setAccel(max_acceleration);   	  // и ограничение ускорения

  base.setSpeed(max_speed);    // Установим ограничение скорости
  base.setAccel(max_acceleration);   	  // и ограничение ускорения

}

void loop() {
  for_kin(h4,h3,h2,h1);
  inv_kin(197.908965, 114.262794,262.817931,30);
  delay(1000);
  // put your main code here, to run repeatedly:

}
