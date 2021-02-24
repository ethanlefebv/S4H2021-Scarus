from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
import cv2
import serial
import argparse

def init_sequence():
    camera = cv2.VideoCapture(0)
    return camera


def main():
    model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"

    # parser = argparse.ArgumentParser()
    # parser.add_argument('-p', required=True, help='Port used for serial communication.')
    # args = parser.parse_args()
    # port = args.p

    camera = init_sequence() # WILL NEED TO PUT THE REST OF THE INIT IN IT (LOL)

    while True:
        box_list = inference(camera,model_path)
        first_nut = box_list[0]
        x = first_nut[0]
        y = first_nut[1]
        nut_class = first_nut[2]



    camera.release()
    return 1

if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
