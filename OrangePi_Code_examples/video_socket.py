import cv2 as cv
import socket
import pickle
import numpy as np

buffer_size = 20000

s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET,socket.SO_SNDBUF,buffer_size)
server_ip = "127.0.0.1"
server_port = 1234
cap = cv.VideoCapture(0)
cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv.CAP_PROP_FRAME_HEIGHT,480)

while True:
	ret,img = cap.read()
	if not ret:
		pass  
	
	cv.imshow('image',img)
	ret,buffer = cv.imencode(".jpg",img,[int(cv.IMWRITE_JPEG_QUALITY),30])
	
	x_as_bytes = pickle.dumps(buffer)
	s.sendto((x_as_bytes),(server_ip,server_port))
	if cv.waitKey(10)==13:
		break
cv.destroyAllWindows()
cap.release()