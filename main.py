from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
from init_stop.init_stop_sequences import *
#from init_stop.init_stop_sequences_test import *


if __name__ == '__main__':
    try:
        model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"
        
        cam, port, ser = init_sequence()
        #cam, port, ser = init_fake_sequence()
        
        #while True:
        nut = get_inference_nut(cam, model_path)
        if nut[2] != -1:
            print_sent_data(send_data(ser, nut_to_string(x, y, nut_class)))
        data = wait_for_data(ser, "")
        #endWhile

        stop_sequence(cam, port, ser)

    except KeyboardInterrupt:
        print('Program stopped by user.')
        stop_sequence(cam, ser, port)
