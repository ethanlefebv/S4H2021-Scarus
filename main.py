from Computer_Vision.inference_pipeline import *
import cv2

def init_sequence():
    camera = cv2.VideoCapture(0)
    return camera


def main():
    model_path = "Computer_Vision/models/yolov4-tiny-416-fp16-nutz.tflite"

    camera = init_sequence() # WILL NEED TO PUT THE REST OF THE INIT IN IT (LOL)

    while True:
        box_list = inference(camera,model_path)

    camera.release()
    return 1

if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
