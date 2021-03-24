from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
from init_stop_sequences.init_stop_sequences import *

#encoding = "utf-8"
#baudrate = 115200

def main():
    model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"

    cam, port, ser = init_fake_sequence()
    #cam, port, ser = init_sequence()

    #while True:
    send_inference_result(cam, model_path)

    data = wait_for_data(ser, "") # Confirmation of Nut

    data = wait_for_data(ser, "") # Angles returned by IK

    data = wait_for_data(ser, "") # Confirmation of motors moved
    #endWhile

    stop_sequence(cam, port, ser)
    return 1

if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
