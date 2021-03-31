from Computer_Vision.inference_pipeline import *
from Communication.serial_functions.functions import *
from init_stop.init_stop_sequences import *


model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"

cam = init_camera(1)

# while True:
nut_x, nut_y, nut_class = get_inference_nut(cam, model_path)
print(nut_x)
print(nut_y)
print(nut_class)