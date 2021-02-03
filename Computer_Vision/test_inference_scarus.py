import tensorflow as tf
import numpy as np
import cv2


def main():
    print('loading of the imports is done')
    print('GO!')
    input_size = 416
    image_path = "./scarus_test.jpg"
    model_path = "./yolov4-416-fp16-pouassons.tflite" # path vers le model .tflite
    image_data = load_saved_image(image_path, input_size)
    print('loaded the image')
    model_output = model_predict(model_path, image_data)
    print('got the model output')
    boxes_tensors, confidence_tensors = get_boxes_tensors(model_output[0], model_output[1],threshold=.85)
    print('got the tensors')
    output = output_parsing(boxes_tensors, confidence_tensors)
    print('parsed the output')
    print('Done')
    print(output)
    return output

def load_saved_image(image_path, input_size):
    original_image = cv2.imread(image_path)
    original_image = cv2.cvtColor(original_image, cv2.COLOR_BGR2RGB)
    image_data = cv2.resize(original_image, (input_size, input_size))
    image_data = image_data / 255.
    images_data = []
    images_data.append(image_data)
    images_data = np.asarray(images_data).astype(np.float32)

    return images_data

def model_predict(model_path, image_data):
    interpreter = tf.lite.Interpreter(model_path=model_path)
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    interpreter.set_tensor(input_details[0]['index'], image_data)
    print('now invoking the model')
    interpreter.invoke()
    print('done invoking')
    model_predictions = [interpreter.get_tensor(output_details[i]['index']) for i in range(len(output_details))]

    return model_predictions


def get_boxes_tensors(boxes_data, scores,threshold=0.99):
    boxes_data_np = np.reshape(boxes_data,(10647,-1))
    scores_np = np.reshape(scores,(10647, -1))
    scores_max_np = np.amax(scores_np, axis=1)
    mask_np = np.argwhere(scores_max_np >= threshold)
    boxes = boxes_data_np[mask_np]
    confidence = scores_np[mask_np]

    return boxes, confidence


def output_parsing(boxes, confidence):
    output = []
    distance_min = 1000 #######TO DO : SEE HOW DISTANT ARE THE NUTS FROM EACH OTHER
    classes = np.array([])
    for box in confidence:
        classes = np.append(classes, np.argmax(confidence[0][0]).astype(np.int8))

    x = np.array([])
    y = np.array([])
    for position in boxes:
        x = np.append(x,position[0][0])
        y = np.append(y,position[0][1])

    boxes = np.array([x,y,classes]).transpose()
    for box_to_compare in boxes:
        same_box_count = 0
        final_box_x = 0
        final_box_y = 0
        for n, box in enumerate(boxes):
            if (((box_to_compare[0]-box[0])**2+(box_to_compare[1]-box[1])**2)<= distance_min) and (box_to_compare[2]==box[2]):
                boxes = np.delete(boxes, n-same_box_count, axis=0)
                same_box_count += 1
                final_box_x += box[0]
                final_box_y += box[1]
        if same_box_count>0:
            x = round(final_box_x/same_box_count,2)
            y = round(final_box_y/same_box_count,2)
            box_class = box_to_compare[2]
            output.append([x,y,box_class])


    return output

if __name__ == '__main__':
    try:
        main()
    except SystemExit:
        pass
