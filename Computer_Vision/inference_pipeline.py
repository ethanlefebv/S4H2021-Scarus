import cv2
import numpy as np
from tflite_runtime.interpreter import Interpreter

INVALID_NUT = -1

def fish_eye_correction(nut):
    dx = nut[0]-207
    dy = nut[1]-207
    nut[0] = int(nut[0]-(0.16*dx))
    nut[1] = int(nut[1]-(0.16*dy))
    return nut

def coord_cam_to_robot(nut):
    nut = fish_eye_correction(nut)
    nut[0] = int(193*nut[0]/416+8-15) #x in mm
    nut[1] = int(-199*nut[1]/416+477-25) #y in mm
    nut[2] = int(nut[2])
    return nut


def get_inference_nut(cam, model_path, should_log):
    if should_log:
        nuts_list = timed_inference(cam, model_path)
    else:
        nuts_list = inference(cam, model_path)

    if nuts_list == []:
        nuts_list = [[0, 0, INVALID_NUT]]

    first_nut = nuts_list[0]
    x = first_nut[0]
    y = first_nut[1]
    nut_class = int(first_nut[2])
    print('Yolo model outputs : ', x, y, nut_class)
    return coord_cam_to_robot(first_nut)


def timed_inference(input_image, model_path):
    start_time = time.time()
    nuts_list = inference(input_image, model_path)
    inf_time = time.time() - start_time
    print("Time spent in inference : {0}".format(inf_time))
    return nuts_list


def inference(input_image, model_path):
    if isinstance(input_image, str):
        image_path = input_image
    else:
        image_path = "camera"
    image_data, original_image = load_image(camera=input_image, input_size=416, image_path=image_path)
    interpreter = Interpreter(model_path=model_path)
    model_output = model_predict(image_data, interpreter)
    boxes_tensors, confidence_tensors = get_boxes_tensors(model_output[0], model_output[1], threshold=0.90)
    output = output_parsing(boxes_tensors, confidence_tensors)
    #show_marked_image(image_data,output) # Uncomment if you want to see the image with predictions
    #print(output)
    return output
    

def load_image(camera=None, input_size=416, image_path='camera',crop=True):
    """
    Load the image or take picture and gives it the size given in inputs and returns the image

    :param input_size: the size that the image must have to fit a certain model (416 for yolov4 and yolo-tiny)
    :param image_path: Path to the image to load, if path == "camera", or unspecified a picture is take from the main
                       camera of the device used.
    :return: a list of the images as np.float32. the list is useful when invoking darknet models
    """
    if image_path == 'camera':
        original_image = take_picture(camera)
    else:
        original_image = cv2.imread(image_path)
        original_image = cv2.cvtColor(original_image, cv2.COLOR_BGR2RGB) # TODO check if the image exists
    if crop == True:
        image_data = crop_frame(original_image)
    else:
        image_data = original_image
    image_data = cv2.resize(image_data, (input_size, input_size))
    image_data = image_data / 255.
    # darknet models need 0-1 range of pixels and not 0-255
    images_list = []
    images_list.append(image_data)
    images_list = np.asarray(images_list).astype(np.float32)
    return images_list, original_image


def take_picture(camera):
    """
    :return: picture taken from the main camera of the device. frame is a numpy array ranging from 0-255
    """
    _, frame = camera.read()
    return frame


def crop_frame(frame, crop_bottom=10, crop_top=38, crop_left=140, crop_right=75):
    """
    :param frame: list of images from take_picture function. A list of numpy arrays of varying shape depending on the
                camera used. To see the  resolution of the image, use frame.shape
    :other params: minimal and maximal value of both axis of the input image
    :return: The same image but cropped with the desired measurements
    """
    frame = frame[crop_bottom:-1-crop_top, crop_left:-1-crop_right]
    return frame


def model_predict(image_data, interpreter):
    """
    :param image_data: the list of images to invoke, output of load_images
    :param interpreter: tensorflow interpreter to invoke
    :return: tensors and meta data about the output of the model
    """
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    interpreter.set_tensor(input_details[0]['index'], image_data)
    interpreter.invoke()
    model_predictions = [interpreter.get_tensor(output_details[i]['index']) for i in range(len(output_details))]

    return model_predictions


def get_boxes_tensors(boxes_data, scores, threshold=0.99):
    """
    :param boxes_data: tensor containing the data of the boxes the model predicted
    :param scores: tensor containing the data of the predictions and scores for all the boxes
    :param threshold: threshold that decides the minimal prediction certainty that the model predicted.
    :return: boxes: numpy array of and only the height, width and center.
           : confidence: numpy array with the confidence of each class for every boxes
    """
    boxes_data_np = np.reshape(boxes_data, (boxes_data.shape[1], -1))
    scores_np = np.reshape(scores, (boxes_data.shape[1], -1))
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
    distance_min = 1000  # to do : SEE HOW DISTANT ARE THE NUTS FROM EACH OTHER
    classes = np.array([])
    for box in confidence:
        classes = np.append(classes, np.argmax(box[0]).astype(np.int8))

    x = np.array([])
    y = np.array([])
    for position in boxes:
        x = np.append(x, position[0][0])
        y = np.append(y, position[0][1])

    """
    Here is the crux of the function. Since the model returns all the possible boxes, so there are a lot of duplicates.
    we iterate through all the boxes and check if there are any that the euclidean distance from their center is too
    close, if so, we join them. After iterating though all the boxes, we make an average of all the too similar boxes and
    add it to the list of final boxes. We then redo for all the boxes left until there are none.  
    """
    boxes = np.array([x, y, classes]).transpose()
    for box_to_compare in boxes:
        same_box_count = 0
        final_box_x = 0
        final_box_y = 0
        for n, box in enumerate(boxes):
            if (((box_to_compare[0] - box[0]) ** 2 + (box_to_compare[1] - box[1]) ** 2) <= distance_min) and (
                    box_to_compare[2] == box[2]):
                boxes = np.delete(boxes, n - same_box_count, axis=0)
                same_box_count += 1
                final_box_x += box[0]
                final_box_y += box[1]
        if same_box_count > 0:
            x = round(final_box_x / same_box_count, 2)
            y = round(final_box_y / same_box_count, 2)
            box_class = box_to_compare[2]
            output.append([x, y, box_class])

    return output

def show_marked_image(image, detected_list):
    """
    :param image: image on which to mark the detected objects
    :param detected_list: list of the coordinate of the center of the objects
    :return: image with the added dots and classes
    """
    marked_image = image[0]
    offset = 7
    WHITE = (0,0,0)
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_size = .5
    font_color = WHITE
    font_thickness = 1
    for detection in detected_list:
        # model outputs on 416 size, we need to scale it to the image size
        x = round(marked_image.shape[1]*detection[0]/416)
        y = round(marked_image.shape[0]*detection[1]/416)
        object_class = int(round(detection[2]))
        if object_class == 0:
            marked_image[y-offset:y+offset,x-offset:x+offset] = [0,0,1]
        elif object_class == 1:
            marked_image[y - offset:y + offset, x - offset:x + offset] = [1, 0, 0]

        cv2.putText(marked_image, str(object_class), (x-offset, y+offset), font, font_size, font_color, font_thickness, cv2.LINE_AA)
    cv2.imshow('unmarked',marked_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()