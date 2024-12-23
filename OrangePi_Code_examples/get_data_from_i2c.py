import smbus
import time

# Укажите номер шины I2C (3 для Orange Pi)
I2C_BUS_NUMBER = 3

# Укажите адрес устройства I2C (для SENSOR_ESP - 0x0A в шестнадцатиричной системе или 10 в десятичной)
DEVICE_ADDRESS = 0x0a

# Создаем объект шины I2C
bus = smbus.SMBus(I2C_BUS_NUMBER)

# Функция для чтения одного байта из устройства
def read_one_byte(device_address):
    try:
        # Читаем один байт из устройства
        byte_data = bus.read_byte(device_address)
        return byte_data
    except Exception as e:
        print(f"Ошибка при чтении данных: {e}")
        return None

if __name__ == "__main__":

    # Чтение одного байта из устройства
    byte_value = read_one_byte(DEVICE_ADDRESS)

    if byte_value is not None:
        print(f"Прочитанный байт: {byte_value}")
