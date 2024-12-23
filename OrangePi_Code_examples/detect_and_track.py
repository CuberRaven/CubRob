import cv2 as cv
import numpy as np

min_contour_size = 250

def detecting_and_track(frame, color_name): # color - "red", "green", "blue"
    # Преобразуем изображение в цветовое пространство HSV
    hsv_frame = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    # Определяем диапазон синего цвета в HSV
    lower_blue = np.array([100, 150, 0])  # Нижний предел
    upper_blue = np.array([140, 255, 255])  # Верхний предел

    # Определяем диапазоны красного цвета в HSV
    lower_red1 = np.array([0, 100, 100])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([160, 100, 100])
    upper_red2 = np.array([180, 255, 255])

    # Определяем диапазон зеленого цвета в HSV
    lower_green = np.array([40, 100, 100])
    upper_green = np.array([80, 255, 255])
    img_bin_raw = []
    color = (255,255,255)
    if color_name == "red":    
        # Создаем маску для синего цвета
        img_bin_raw1 = cv.inRange(hsv_frame, lower_red1, upper_red1)
        img_bin_raw2 = cv.inRange(hsv_frame, lower_red2, upper_red2)
        img_bin_raw = cv.bitwise_or(img_bin_raw1, img_bin_raw2)
        color = (0,0,255)

    elif color_name == "green":
        img_bin_raw = cv.inRange(hsv_frame, lower_green, upper_green)
        color = (0,255,0)
    elif color_name == "blue":
        img_bin_raw = cv.inRange(hsv_frame, lower_blue, upper_blue)
        color = (255,0,0)
    else:
        raise ValueError("Select right color_name! [red, green, blue]")

    cnts, hierarchy = cv.findContours(img_bin_raw,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
    cross_x = -1
    cross_y = -1
    if cnts:
        c = max(cnts, key = cv.contourArea)
        if cv.contourArea(c) >= min_contour_size:
            x, y, w, h = cv.boundingRect(c)
            cv.rectangle(frame, (x, y), (x + w, y + h), color, 2)  # Рисуем прямоугольник
            draw_cross(frame, int(x+w/2), int(y+h/2), 25, color) # Отметим центр крестиком
            
            cross_x = int(x+w/2)
            cross_y = int(y+h/2)

    return frame, cross_x, cross_y # Вернем кадр и координаты центра обнаруженного объекта

def erode_dilate(img, kernel_sz):
    kernel = cv.getStructuringElement(cv.MORPH_RECT,(kernel_sz,kernel_sz))
    cv.erode(img,kernel)
    cv.dilate(img,kernel)
    return img

def draw_cross(frame, x, y, size, color):
    cv.line(frame,(int(x-size/2),int(y)), (int(x+size/2),int(y)),color,2)
    cv.line(frame,(int(x),int(y-size/2)), (int(x),int(y+size/2)),color,2)
    

# Начинаем захват видео, в скобках указывается номер видеокамеры (ls /dev/video* для просмотра всех видеокамер)
cap = cv.VideoCapture(0)

# Мы можем указывать параметры захвата видео
# Установим разрешение 640х480 для быстродействия, к тому же исходное 4к-изображение может не влезть в экран
ret = cap.set(cv.CAP_PROP_FRAME_WIDTH,640)
ret = cap.set(cv.CAP_PROP_FRAME_HEIGHT,480)

# В бесконечном цикле
while True:
    ret, frame = cap.read() # Принимаем кадр
    frame_height, frame_width, chl = np.shape(frame)
    detected, x, y  = detecting_and_track(frame, "blue")
    offset = x - frame_width/2
    cv.imshow('detected', detected) # Выводим его в окно
    print(f"OFFSET: {offset}")
    cv.waitKey(30) # Ждем 30мс, обеспечивая нужную нам частоту кадров

    # offset/320 * 255 - left
    #right = -left