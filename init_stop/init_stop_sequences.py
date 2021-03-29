from Communication.serial_functions.functions import *
import cv2
import time
import argparse
import serial

def init_sequence():
    baudrate = 115200
    camera_index, port = init_parse()
    cam = init_camera(camera_index)
    ser = init_port(port, baudrate)
    init_opencr(ser)
    return cam, port, ser


def init_parse():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', required=True, type=int, help='Index of the camera.')
    parser.add_argument('-p', required=True, help='Port used for serial communication.')
    args = parser.parse_args()
    camera_index = args.c
    port = args.p
    return camera_index, port


def init_camera(camera_index):
    camera = cv2.VideoCapture(camera_index)
    return camera


def init_port(port, baudrate):
    print("Opening Serial communication with {0}.".format(port))
    ser = serial.Serial(port, baudrate)
    ser.flush()
    return ser


def init_opencr(ser):
    msg = get_data(ser)
    # Confirm the OpenCR is waiting to start
    if msg != "Waiting for the START command.":
        raise SerialError("The OpenCR is not waiting to start. Try restarting it.")

    print_sent_data(send_data(ser, "START"))
    data = wait_for_data(ser, "Starting the program.")


def stop_sequence(cam, port, ser):
    print_sent_data(send_data(ser, "STOP"))
    print_received_data(get_data(ser))

    cam.release()
    print("Closing Serial communication with {0}.".format(port))
    ser.close()