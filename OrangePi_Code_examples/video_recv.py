import cv2
import socket
import numpy
import pickle

buffer_size = 20000

# Создадим сокет
s=socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
ip="127.0.0.1"
port=1234
s.bind((ip,port))

while True:
    x=s.recvfrom(buffer_size) # Принимаем данные
    data = pickle.loads(x[0]) # Преобразуем их в переменную data
    data = cv2.imdecode(data, cv2.IMREAD_COLOR)
    
    cv2.imshow('server', data) # Показываем полученное изображение
    if cv2.waitKey(10) == 13: # Если нажать Enter, выйдем из цикла и закроем окно
        break
cv2.destroyAllWindows()