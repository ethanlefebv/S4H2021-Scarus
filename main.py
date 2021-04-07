from Computer_Vision.inference_pipeline import *
from init_stop.init_stop_sequences import *
import time
#from init_stop.init_stop_sequences_test import *


if __name__ == '__main__':
    try:
        model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"
        
        cam, port, ser = init_sequence()
        #cam, port, ser = init_fake_sequence()
        
        while True:
            nut = coord_cam_to_robot(get_inference_nut(cam, model_path, False))
            nut_x = nut[0]
            nut_y = nut[1]
            nut_class = nut[2]
            if nut_class != INVALID_NUT:
                print_sent_data(send_data(ser, nut_to_string(nut_x, nut_y, nut_class)))
                wait_for_data(ser, "Picked")
                wait_for_data(ser, "Done")

        stop_sequence(cam, port, ser)

    except KeyboardInterrupt:
        print('Program stopped by user.')
        stop_sequence(cam, port, ser)
        
    except SerialError as se:
        print(se)
