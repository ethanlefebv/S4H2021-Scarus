from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
from init_stop_sequences.init_stop_sequences import *


#encoding = "utf-8"
#baudrate = 115200

if __name__ == '__main__':
    try:
        model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"
        
        cam, port, ser = init_sequence()
        #cam, port, ser = init_fake_sequence()
        
        #while True:
        send_inference_result(cam, model_path)
        data = wait_for_data(ser, "")
        #endWhile

        stop_sequence(cam, port, ser)

    except KeyboardInterrupt:
        print('Program stopped by user.')
        stop_sequence(cam, ser, port)
