from tflite_runtime.interpreter import Interpreter
import numpy as np
import cv2


def main():
    print('loading of the imports is done')
    print('GO!')
    image_path = "./test_img.jpg"
    model_path = "models/yolov4-416-fp16-pouassons.tflite"
    image_data = load_image()
    image_data = crop_frame(image_data, 0,200,0,400)
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


def load_image(input_size=416,image_path='camera'):
    """
    :param input_size: the size that the image must have to fit a certain model (416 for yolov4 and yolo-tiny)
    :param image_path: Path to the image to load, if path == "camera", or unspecified a picture is take from the main
                       camera of the device used.
    :return: a list of the images as np.float32. the list is useful when invoking darknet models
    """
    if image_path == 'camera':
        original_image = take_picture()
    else:
        original_image = cv2.imread(image_path)
        original_image = cv2.cvtColor(original_image, cv2.COLOR_BGR2RGB)
    image_data = cv2.resize(original_image, (input_size, input_size))
    image_data = image_data / 255. # darknet models need 0-1 range of pixels and not 0-255
    images_data = []
    images_data.append(image_data)
    images_data = np.asarray(images_data).astype(np.float32)
    return images_data

def take_picture():
    """
    :return: picture taken from the main camera of the device. frame is a numpy array ranging from 0-255
    """
    cap = cv2.VideoCapture(0)
    _, frame = cap.read()
    cap.release()
    cv2.imwrite('last_img.jpg', frame)
    return frame

def crop_frame(frame, min_x, max_x, min_y, max_y):
    """
    :param frame: list of images from take_picture function. A list of numpy arrays of varying shape depending on the
                camera used. To see the  resolution of the image, use frame.shape
    :other params: minimal and maximal value of both axis of the input image
    :return: The same image but cropped with the desired measurements
    """
    crop_img = []
    for image in frame:
        crop_img.append(image[min_y:max_y, min_x:max_x])
        cv2.imwrite('last_image.jpg', image[min_y:max_y, min_x:max_x])
    return crop_img


def model_predict(model_path, image_data):
    """
    :param model_path: path to the model to use for the inference
    :param image_data: the list of images to invoke, output of load_images
    :return:
    """
    interpreter = Interpreter(model_path=model_path)
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
    """
    :param boxes_data: tensor containing the data of the boxes the model predicted
    :param scores: tensor containing the data of the predictions and scores for all the boxes
    :param threshold: threshold that decides the minimal prediction certainty that the model predicted.
    :return: boxes: numpy array of and only the height, width and center.
           : confidence: numpy array with the confidence of each class for every boxes
    """
    boxes_data_np = np.reshape(boxes_data,(10647,-1))
    scores_np = np.reshape(scores,(10647, -1))
    scores_max_np = np.amax(scores_np, axis=1)
    mask_np = np.argwhere(scores_max_np >= threshold)
    boxes = boxes_data_np[mask_np]
    confidence = scores_np[mask_np]

    return boxes, confidence


def output_parsing(boxes, confidence):
    """
    :param boxes: list of boxes with their height width and center
    :param confidence: confidence for each class for each box
    :return: a list of the center coordinates for each prediction and the class associated with it
    """
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

    """
    Here is the crux of the function. Since the model returns all the possible boxes, so there are a lot of duplicates.
    we iterate through all the boxes and check if there are any that the euclidean distance from their center is too
    close, if so, we join them. After iterating though all the boxes, we make an average of all the too similar boxes and
    add it to the list of final boxes. We then redo for all the boxes left until there are none.  
    """
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
