# Добавим простой пороговый фильтр

import cv2 as cv
import numpy as np

cap = cv.VideoCapture(0)
ret = cap.set(cv.CAP_PROP_FRAME_WIDTH,640)
ret = cap.set(cv.CAP_PROP_FRAME_HEIGHT,480)

thresh = 127 # Порог. Пиксели с яркостью ниже порога становятся черными, выше - белыми

while True:
    ret, raw = cap.read()

    # Изменим цветовую схему изображения с цветного на оттенки серого
    img = cv.cvtColor(raw, cv.COLOR_BGR2GRAY)
    orig = cv.cvtColor(raw, cv.COLOR_BGR2GRAY)

    # Превращаем изображение в оттенках серого в бинарное. 
    # В бинарном изображении пиксели бывают только двух цветов - черного и белого
    ret,th_frame = cv.threshold(img,thresh,255,cv.THRESH_BINARY)

    # Соединим бинарное изображение с кадром в оттенках серого, чтобы наглядно увидеть разницу
    output = np.concatenate((orig, th_frame), axis=1)
    cv.imshow('output', output)
    cv.waitKey(30)