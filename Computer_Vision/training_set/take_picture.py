import cv2
import numpy as np



cap = cv2.VideoCapture(0)
if cap.isOpened():
    _,frame = cap.read()
    cap.release()
    #cv2.imwrite('test_img.jpg', frame)

