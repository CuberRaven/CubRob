import cv2 as cv
import numpy as np

# Начинаем захват видео, в скобках указывается номер видеокамеры (ls /dev/video* для просмотра всех видеокамер)
cap = cv.VideoCapture(0)

# Мы можем указывать параметры захвата видео
# Установим разрешение 640х480 для быстродействия, к тому же исходное 4к-изображение может не влезть в экран
ret = cap.set(cv.CAP_PROP_FRAME_WIDTH,640)
ret = cap.set(cv.CAP_PROP_FRAME_HEIGHT,480)

# В бесконечном цикле
while True:
    ret, frame = cap.read() # Принимаем кадр

    cv.imshow('output', frame) # Выводим его в окно
    cv.waitKey(30) # Ждем 30мс, обеспечивая нужную нам частоту кадров