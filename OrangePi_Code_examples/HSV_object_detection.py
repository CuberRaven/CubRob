import cv2 as cv
import numpy as np

class HSV_detection():

    def __init__(self):
        self.trbar_upd = True

        # Цвета, которыми будем обводить объекты соответствующих цветов
        self.red =   (10,10,220)
        self.green = (10,220,10)
        self.blue =  (220,10,10)
        self.line_width = 2

        self.max_hue = 179
        self.max_tb_val = 255

        # Начальные значения границ HSV для всех цветов, которые будут применены при старте программы
        self.red_u_bnd_h = 50
        self.red_u_bnd_s = 255
        self.red_u_bnd_v = 165
        self.red_l_bnd_h = 0
        self.red_l_bnd_s = 80
        self.red_l_bnd_v = 110

        self.green_u_bnd_h = 90
        self.green_u_bnd_s = 255
        self.green_u_bnd_v = 255
        self.green_l_bnd_h = 51
        self.green_l_bnd_s = 100
        self.green_l_bnd_v = 60

        self.blue_u_bnd_h = 121
        self.blue_u_bnd_s = 255
        self.blue_u_bnd_v = 255
        self.blue_l_bnd_h = 91
        self.blue_l_bnd_s = 100
        self.blue_l_bnd_v = 80
        return
    
    # Функция для обновления границ HSV. Вызывается, когда мы двигаем трекбар
    def update_tbar(self):

        self.red_u_bnd_h = cv.getTrackbarPos('Up.hue', 'HSV Red Bounds')
        self.red_u_bnd_s = cv.getTrackbarPos('Up.sat', 'HSV Red Bounds')
        self.red_u_bnd_v = cv.getTrackbarPos('Up.val', 'HSV Red Bounds')
        self.red_l_bnd_h = cv.getTrackbarPos('Low.hue', 'HSV Red Bounds')
        self.red_l_bnd_s = cv.getTrackbarPos('Low.sat', 'HSV Red Bounds')
        self.red_l_bnd_v = cv.getTrackbarPos('Low.val', 'HSV Red Bounds')

        self.green_u_bnd_h = cv.getTrackbarPos('Up.hue', 'HSV Green Bounds')
        self.green_u_bnd_s = cv.getTrackbarPos('Up.sat', 'HSV Green Bounds')
        self.green_u_bnd_v = cv.getTrackbarPos('Up.val', 'HSV Green Bounds')
        self.green_l_bnd_h = cv.getTrackbarPos('Low.hue', 'HSV Green Bounds')
        self.green_l_bnd_s = cv.getTrackbarPos('Low.sat', 'HSV Green Bounds')
        self.green_l_bnd_v = cv.getTrackbarPos('Low.val', 'HSV Green Bounds')

        self.blue_u_bnd_h = cv.getTrackbarPos('Up.hue', 'HSV Blue Bounds')
        self.blue_u_bnd_s = cv.getTrackbarPos('Up.sat', 'HSV Blue Bounds')
        self.blue_u_bnd_v = cv.getTrackbarPos('Up.val', 'HSV Blue Bounds')
        self.blue_l_bnd_h = cv.getTrackbarPos('Low.hue', 'HSV Blue Bounds')
        self.blue_l_bnd_s = cv.getTrackbarPos('Low.sat', 'HSV Blue Bounds')
        self.blue_l_bnd_v = cv.getTrackbarPos('Low.val', 'HSV Blue Bounds')

        return
    

    def tbar_clbck(self, value):
        self.update_tbar()
        return

    # На изображении могут быть распознаны не только области нужного нам цвета, но и отдельные пиксели (шумы), которые нам учитывать не нужно
    # Эрозия и дилатация помогут избавиться от шумов, и оставят распознанными только крупные области
    def erode_dilate(self,img, kernel_sz):
        kernel = cv.getStructuringElement(cv.MORPH_RECT,(kernel_sz,kernel_sz))
        cv.erode(img,kernel)
        cv.dilate(img,kernel)
        return img

    # Найти контуры распознанного объекта и обвести нужным нам цветом
    def mark_detected(self, img_bin, dst, color):
        cnts, hierarchy = cv.findContours(img_bin,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_NONE)
        cv.drawContours(dst,cnts,-1,color, self.line_width)
        return

    # Функция для создания окна с трекбарами для каждого цвета
    def HSV_trackbar(self, name, u_bnd_h,u_bnd_s,u_bnd_v,l_bnd_h,l_bnd_s,l_bnd_v):
        cv.namedWindow(name)

        cv.createTrackbar("Up.hue",name,u_bnd_h,self.max_hue, self.tbar_clbck)
        cv.createTrackbar("Up.sat",name,u_bnd_s,self.max_tb_val, self.tbar_clbck)
        cv.createTrackbar("Up.val",name,u_bnd_v,self.max_tb_val, self.tbar_clbck)
        cv.createTrackbar("Low.hue",name,l_bnd_h,self.max_hue, self.tbar_clbck)
        cv.createTrackbar("Low.sat",name,l_bnd_s,self.max_tb_val, self.tbar_clbck)
        cv.createTrackbar("Low.val",name,l_bnd_v,self.max_tb_val, self.tbar_clbck)
        return

    def detecting(self, img, img_hsv, u_bnd_h, u_bnd_s, u_bnd_v,l_bnd_h, l_bnd_s, l_bnd_v,color, name):
        
        u_bnd = (u_bnd_h,u_bnd_s,u_bnd_v) # Верхние HSV границы
        l_bnd = (l_bnd_h,l_bnd_s,l_bnd_v) # Нижние HSV границы
        img_bin = cv.inRange(img_hsv,l_bnd,u_bnd)

        # Иногда красный цвет может быть принят за фиолетовый, их значения оттенка находятся на противоположных концах шкалы
        if name == "Red":
            purple_low_bnd = self.max_hue - 40
            u_bnd_add = (self.max_hue - 1,u_bnd_s,u_bnd_v)
            l_bnd_add = (purple_low_bnd,l_bnd_s,l_bnd_v)
            purple = cv.inRange(img_hsv,l_bnd_add,u_bnd_add) # Распознаем фиолетовые области на изображении
            img_bin += purple # И добавим их к красным
        
        img_bin = self.erode_dilate(img_bin,7)
        self.mark_detected(img_bin, img, color)
        return


if __name__ == "__main__":
    # Захват видео
    cap = cv.VideoCapture(0) 
    cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT,480)

    detector = HSV_detection()

    # Создадим трекбары для управления границами HSV-параметров
    detector.HSV_trackbar("HSV Red Bounds", detector.red_u_bnd_h,detector.red_u_bnd_s,detector.red_u_bnd_v,
                                    detector.red_l_bnd_h,detector.red_l_bnd_s,detector.red_l_bnd_v)
    detector.HSV_trackbar("HSV Green Bounds", detector.green_u_bnd_h,detector.green_u_bnd_s,detector.green_u_bnd_v,
                                        detector.green_l_bnd_h,detector.green_l_bnd_s,detector.green_l_bnd_v)
    detector.HSV_trackbar("HSV Blue Bounds", detector.blue_u_bnd_h,detector.blue_u_bnd_s,detector.blue_u_bnd_v,
                                    detector.blue_l_bnd_h,detector.blue_l_bnd_s,detector.blue_l_bnd_v)

    while True:
        ret, img = cap.read()
        if not ret:
            pass
        img_hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)
        detector.detecting(img,img_hsv, detector.red_u_bnd_h,detector.red_u_bnd_s,detector.red_u_bnd_v,
                                        detector.red_l_bnd_h,detector.red_l_bnd_s,detector.red_l_bnd_v,detector.red,"Red")
        detector.detecting(img,img_hsv, detector.green_u_bnd_h,detector.green_u_bnd_s,detector.green_u_bnd_v,
                                        detector.green_l_bnd_h,detector.green_l_bnd_s,detector.green_l_bnd_v,detector.green,"Green")
        detector.detecting(img,img_hsv, detector.blue_u_bnd_h,detector.blue_u_bnd_s,detector.blue_u_bnd_v,
                                        detector.blue_l_bnd_h,detector.blue_l_bnd_s,detector.blue_l_bnd_v,detector.blue,"blue")
    
        cv.imshow("output", img)
        cv.waitKey(30)