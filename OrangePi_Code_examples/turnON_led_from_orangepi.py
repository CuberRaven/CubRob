import smbus2
import struct

# Адрес ведомого устройства ESP_SENSOR
DEVICE_ADDRESS = 0x0a
# Укажите номер шины I2C (3 для Orange Pi)
I2C_BUS_NUMBER = 3

# Инициализация шины I2C
bus = smbus2.SMBus(I2C_BUS_NUMBER)

# Отправка значений красного, зеленого и синего каналов на ESP_SENSOR по I2C
def send_led(red, green, blue):
	data = bytearray([red, green, blue])
	bus.write_i2c_block_data(DEVICE_ADDRESS, 0, list(data))

# Пример использования
if __name__ == "__main__":
    try:
        send_led(0,255,255)
        print("Bytes sent!")
    except Exception as e:
        print(f"Ошибка: {e}")
