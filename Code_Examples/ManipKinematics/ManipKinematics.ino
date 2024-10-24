#include <ServoDevs.h> 
#include <ServoSmooth.h>
#include <BluetoothSerial.h>

ServoSmooth base; // Объявим переменную сервопривода
ServoSmooth shoulder; // Объявим переменную сервопривода
ServoSmooth elbow; // Объявим переменную сервопривода
ServoSmooth wrist; // Объявим переменную сервопривода

BluetoothSerial ESP_BT; // Объект Bluetooth

bool flag = true;
const double EPSL = 0.0001;
long long int timestamp = 0;
int tick_timestamp = 0;

#define DEBUG 1

/* Длины звеньев в мм */
uint32_t l1 = 87;
uint32_t l2 = 105;
uint32_t l3 = 76; 
uint32_t l4 = 115; 

double h1 = 120.; // wrist
double h2 = 60.;  // elbow
double h3 = 30.; // shoulder
double h4 = 30.; // base

int max_speed = 100;
double max_acceleration = 0.2;
const int JOINTS_NUM = 4;

double fk_solution[4] = {0}; // Решение прямой задачи кинематики
double ik_solution[4] = {h4, h3, h2, h1}; // Решение обратной задачи кинематики

inline double sqr(double x) {
  return x*x;
}

inline double degToRad(double x) {
  return (double)(x/180)*PI;
}

inline double radToDeg(double x) {
  return (double)(x/PI)*180;
}

/* Углы шарниров могут принимать значения 0-180 градусов */
bool check_bounds(double * pos) {
  if (sizeof(pos) < JOINTS_NUM) return false;
  for (int i = 0; i < JOINTS_NUM; i++) {
    if ((pos[i] < 0) || (pos[i] > 180)) {
      return false;
    }
  }
  return true;
}

/* Решение прямой задачи кинематики должно совпадать с нужным нам положением манипулятора */
bool is_equal(double * fk_sol, double * target_pose) {
  for (int i = 0; i < JOINTS_NUM; i++) {
    Serial.printf("[%f]",fabs(fk_sol[i] - target_pose[i]));
    if (fabs(fk_sol[i] - target_pose[i]) > EPSL) return false;
  }
  Serial.printf("CORRECT SOLUTION!\n");
  return true;
}

void set_ik_solution(double * solution) {
  for (int i = 0; i < JOINTS_NUM; i++) {
    ik_solution[i] = solution[i];
  }
}

/* Для проверки каждого из 4-х возможных решений, в итоге выбирается то, которое достигает целевого положения. */
bool check_solution(double * solution, double * target_pose) {
  for_kin(solution[0],solution[1],solution[2],solution[3]);
  if (is_equal(fk_solution, target_pose) && check_bounds(solution)) {
    set_ik_solution(solution);
    return true;
  } else {
    return false;
  }
}

inline double calcR(double th1, double th2, double th3) {
  double r_th1 = degToRad(th1);
  double r_th2 = degToRad(th2);
  double r_th3 = degToRad(th3);
  return (double)(l2*cos(r_th1) + l3*cos((PI/2)+r_th1-r_th2) + l4*cos(PI+r_th1-r_th3-r_th2));
}

inline double calcH(double th1, double th2, double th3) {
  double r_th1 = degToRad(th1);
  double r_th2 = degToRad(th2);
  double r_th3 = degToRad(th3);
  return (double)(l1 + l2*sin(r_th1) + l3*sin((PI/2)+r_th1-r_th2) + l4*sin(PI+r_th1-r_th3-r_th2));
}

void for_kin(double th1, double th2, double th3, double th4) {
  double r_th1 = degToRad(th1);
  double R = calcR(th2, th3, th4);
  double H = calcH(th2, th3, th4);

  double x = R*cos(r_th1);
  double y = R*sin(r_th1);
  double z = H;

  double phi = PI + degToRad(th2 - th3 - th4);
  fk_solution[0] = x;
  fk_solution[1] = y;
  fk_solution[2] = z;
  fk_solution[3] = radToDeg(phi);
  Serial.printf("FK: x = %f mm, y = %f mm, z = %f mm phi = %f\n", x, y, z, radToDeg(phi));
}

bool inv_kin(double x, double y, double z, double phi) {

  double target_pose[4] = {x,y,z,phi}; // для проверки прямой кинематики
  double solution1[4] = {0};
  double solution2[4] = {0};
  double solution3[4] = {0};
  double solution4[4] = {0};

  bool have_solution = false;

  /* Угол th1 базового шарнира: */
  phi = degToRad(phi);
  double th1 = 0;
  if (y > 0) {
    th1 = acos(x/(sqrt(sqr(x)+sqr(y))));
  }
  else {
    th1 = PI - acos(x / (sqrt(sqr(x) + sqr(y))));
  }
  Serial.printf("\nTH1: %f\n", radToDeg(th1));
  solution1[0] = radToDeg(th1);
  solution2[0] = radToDeg(th1);
  solution3[0] = radToDeg(th1);
  solution4[0] = radToDeg(th1);

  /* Угол th3 */
  /* Отрезок от шарнира base до шарнира wrist */
  double x_ = x - l4*cos(th1)*cos(phi);
  double y_ = y - l4*sin(th1)*cos(phi);
  double z_ = z - l1 - l4*sin(phi);
  double XYZ = sqr(x_) + sqr(y_) + sqr(z_);
  double th3_1 = asin((XYZ - sqr(l2) - sqr(l3))/(2*l2*l3)); // Дробь, передаваемая в asin, должна принимать значения [-1;1]
  if (isnan(th3_1)) {
    Serial.println("Точка недостижима!");
    return false; // В противном случае угол будет равен NaN, и задача решений иметь не будет
  }
  double th3_2 = PI - th3_1;

  solution1[2] = radToDeg(th3_1);
  solution2[2] = radToDeg(th3_1);
  solution3[2] = radToDeg(th3_2);
  solution4[2] = radToDeg(th3_2);

  Serial.printf("TH3: %f or2 %f\n", radToDeg(th3_1), radToDeg(th3_2));
  /* В плоскости манипулятора */

  /* 2 значения th3 для двух взаимных расположений звеньев l2 и l3 , симметричных относительно отрезка OA (от начала координат до шарнира wrist) */
  double a_1 = l2 + l3 * sin(th3_1);
  double a_2 = l2 + l3 * sin(th3_2);
  double b_1 = l3 * cos(th3_1);
  double b_2 = l3 * cos(th3_2);

  double alpha_1 = atan2(b_1, a_1);
  double alpha_2 = atan2(b_2, a_2);
  double beta = atan2(z_, sqrt(sqr(x_) + sqr(y_)));
  double th2 = 0;

  double th2_1 = beta - alpha_1;
  double th2_2 = PI - th2_1;
  double th2_3 = beta + alpha_2;
  double th2_4 = PI - th2_3;

  solution1[1] = radToDeg(th2_1);
  solution2[1] = radToDeg(th2_2);
  solution3[1] = radToDeg(th2_3);
  solution4[1] = radToDeg(th2_4);

  Serial.printf("TH2: %f or2 %f or3 %f or4 %f\n", radToDeg(th2_1), radToDeg(th2_2), radToDeg(th2_3), radToDeg(th2_4));

  double th4_1 = PI - phi - th3_1 + th2_1;
  double th4_2 = PI - phi - th3_1 + th2_2;
  double th4_3 = PI - phi - th3_2 + th2_3;
  double th4_4 = PI - phi - th3_2 + th2_4;

  solution1[3] = radToDeg(th4_1);
  solution2[3] = radToDeg(th4_2);
  solution3[3] = radToDeg(th4_3);
  solution4[3] = radToDeg(th4_4);

  Serial.printf("TH4: %f or2 %f or3 %f or4 %f\n", radToDeg(th4_1), radToDeg(th4_2), radToDeg(th4_3), radToDeg(th4_4));

  Serial.printf("\nCHECK SOLUTIONS:\n");

  Serial.printf("1:\n");
  if (check_solution(solution1, target_pose)) have_solution = true;
  Serial.printf("2:\n");
  if (check_solution(solution2, target_pose)) have_solution = true;
  Serial.printf("3:\n");
  if (check_solution(solution3, target_pose)) have_solution = true;
  Serial.printf("4:\n");
  if (check_solution(solution4, target_pose)) have_solution = true;

  if (!have_solution)
    Serial.println("Точка недостижима!");
  return have_solution;
}

void moveToPose(double * pose) {

  wrist.setTargetDeg(pose[3]);
  elbow.setTargetDeg(pose[2]);
  shoulder.setTargetDeg(pose[1]);
  base.setTargetDeg(pose[0]);
}

void moveToHome() {
  wrist.attach(PIN_MANIP_0, h1);
  elbow.attach(PIN_MANIP_2, h2);
  shoulder.attach(PIN_MANIP_3, h3);
  base.attach(PIN_MANIP_4, h4);
  
  wrist.smoothStart();
  elbow.smoothStart();
  shoulder.smoothStart();
  base.smoothStart(); // Плавно движемся в стартовую позицию 

  wrist.setSpeed(max_speed);    // Установим ограничение скорости
  wrist.setAccel(max_acceleration);   	  // и ограничение ускорения

  elbow.setSpeed(max_speed);    // Установим ограничение скорости
  elbow.setAccel(max_acceleration);   	  // и ограничение ускорения

  shoulder.setSpeed(max_speed);    // Установим ограничение скорости
  shoulder.setAccel(max_acceleration);   	  // и ограничение ускорения

  base.setSpeed(max_speed);    // Установим ограничение скорости
  base.setAccel(max_acceleration);   	  // и ограничение ускорения

  base.setAutoDetach(false);
  shoulder.setAutoDetach(false);
  elbow.setAutoDetach(false);
  wrist.setAutoDetach(false);
}

/*
 * С помощью bluetooth-терминала на робот отправляется строка с нужными координатами 
 * через точку с запятой: x;y;z;phi
 */
void get_target_from_BT() {
  String incoming = "";
  while (ESP_BT.available()) { // Проверяем, не получили ли мы что-либо по Bluetooth
    incoming += (char)ESP_BT.read(); // Считываем данные
  }
  if (incoming.length() > 0) {
    Serial.print(incoming);

    /* Расположение точек с запятой в строке */
    int ind1 = incoming.indexOf(';');
    int ind2 = incoming.indexOf(';',ind1+1);
    int ind3 = incoming.indexOf(';',ind2+1);

    /* Найдем в строке все координаты */
    double x = incoming.substring(0, ind1).toDouble();
    double y = incoming.substring(ind1+1, ind2).toDouble();
    double z = incoming.substring(ind2+1, ind3).toDouble();
    double phi = incoming.substring(ind3+1).toDouble();
    Serial.printf("X:%f Y:%f Z:%f PHI:%f\n", x, y, z, phi);

    /* Решим обратную задачу кинематики */
    inv_kin(x, y, z, phi);

    /* Зададим сервоприводам углы, вычисленные при решении обратной задачи кинематики */
    moveToPose(ik_solution);
  } 
}


void setup() {
  Serial.begin(115200);
  moveToHome();

  ESP_BT.begin("ESP32_manipulator_Control"); // Имя хоста Bluetooth
  Serial.println("Bluetooth Device is Ready to Pair");
  
}

void loop() {
  /* Получаем углы по bluetooth */
  get_target_from_BT();
  
  /* Двигаем все сервоприводы, дергая их каждые 10 миллисекунд */
  if (millis() - tick_timestamp > 10) {
    
    wrist.tickManual();
    elbow.tickManual();
    shoulder.tickManual();
    base.tickManual();
    tick_timestamp = millis();
  }
}

