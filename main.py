from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
import cv2
import serial
import argparse
import time

#encoding = "utf-8"
#baudrate = 115200

def init_opencr(ser):
    msg = get_data(ser)
    print_sent_data(send_data(ser, "START"))
    print_received_data(get_data(ser))


def init_sequence(camera_index, port):
    camera = cv2.VideoCapture(camera_index)

    print("Opening Serial communication with {0}.".format(port))
    ser = serial.Serial(port, baudrate)
    ser.flush()
    init_opencr(ser)
    return camera, ser


def main():
    model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"

    parser = argparse.ArgumentParser()
    parser.add_argument('-c', required=True, type=int, help='Index of the camera.')
    parser.add_argument('-p', required=True, help='Port used for serial communication.')
    args = parser.parse_args()
    camera_index = args.c
    port = args.p

    camera, ser = init_sequence(camera_index, port)

    start_time = time.time()
    box_list = inference(camera, model_path)
    inf_time = time.time() - start_time
    print("Time spent in inference : {0}".format(inf_time))

    first_nut = box_list[0]
    x = first_nut[0]
    y = first_nut[1]
    nut_class = int(first_nut[2])
    print('Yolo model outputs : ', x, y, nut_class)
    print_sent_data(send_data(ser, nut_to_string(x, y, nut_class)))
    print_received_data(get_data(ser)) # Confirmation of Nut
    print_received_data(get_data(ser)) # Angles returned by IK
    print_received_data(get_data(ser)) # Confirmation of motors moved
    print_sent_data(send_data(ser, "STOP"))
    print_received_data(get_data(ser))

    camera.release()
    print("Closing Serial communication with {0}.".format(port))
    ser.close()
    return 1

if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
