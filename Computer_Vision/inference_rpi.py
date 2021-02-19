from tflite_runtime.interpreter import Interpreter
from Computer_Vision.inference_pipeline import *
import time



def main():
    camera = cv2.VideoCapture(0)
    print('loading of the imports is done')
    print('GO!')
    start = time.time()
    image_path = "camera"
    model_path = "models/yolov4-tiny-416-fp16-nutz.tflite"
    image_data, original_image = load_image(camera=camera, input_size=416, image_path=image_path)
    print('loaded the image')
    interpreter = Interpreter(model_path=model_path)
    model_output = model_predict(image_data, interpreter)
    print('got the model output')
    boxes_tensors, confidence_tensors = get_boxes_tensors(model_output[0], model_output[1],threshold=.98)
    print('got the tensors')
    output = output_parsing(boxes_tensors, confidence_tensors)
    print('parsed the output')
    print('Done')
    print(output)
    print(time.time() - start)
    return output




if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
