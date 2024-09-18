
/* Бодрейт должен совпадать с тем значением baud, которое выбрано в Serial Monitor */
int baudrate = 115200;
int messageCounter = 0;

void setup() {
  /* Запустим последовательный порт */
  Serial.begin(baudrate);

}

void loop() {
  /* 
     Функция Serial.print() выводит в последовательный порт строку или значение какой-либо переменной,
     которую мы хотим отслеживать.
   */
  Serial.print("Новое ");

  /* Функция Serial.println() делает то же самое, но в конце переходит на следующую строку */
  Serial.println("сообщение!");

  /* 
     Функция Serial.printf() позволяет нам самим формировать выводимое сообщение.
     Здесь \n - символ перевода строки, а %d - значение целочисленных переменных,
     которые мы перечисляем через запятую в той же последовательности, в которой
     расположены "%d" в строке. 
   */
  Serial.printf("Счетчик: %d.\nТекущий бодрейт: %d. \n\n",messageCounter, baudrate);

  messageCounter++;
  delay(1000);

}
