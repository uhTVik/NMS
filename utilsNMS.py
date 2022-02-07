from datetime import datetime
import ast
from pathlib import Path
import numpy as np
import cv2
from random import randint, uniform

max_image_size = 256


def create_dataset_NMS(number_of_tests=10):
    number_of_candidates = 9
    change = 20

    dir = "tests/"
    Path(dir).mkdir(parents=True, exist_ok=True)
    file_name = "test_" + str(datetime.now()).replace(" ", "")
    for t in range(number_of_tests):
        boxes = []
        scores = []
        target_box = [randint(0, max_image_size-1), randint(0, max_image_size-1), randint(0, max_image_size-1),
                      randint(0, max_image_size-1)]
        if target_box[0] > target_box[2]:
            target_box = [target_box[2], target_box[1], target_box[0], target_box[3]]
        if target_box[1] > target_box[3]:
            target_box = [target_box[0], target_box[3], target_box[2], target_box[1]]
        for i in range(number_of_candidates):
            cur_box = [min(max(target_box[0] + randint(-change, change), 0), max_image_size-1),
                       min(max(target_box[1] + randint(-change, change), 0), max_image_size-1),
                       min(max(target_box[2] + randint(-change, change), 0), max_image_size-1),
                       min(max(target_box[3] + randint(-change, change), 0), max_image_size-1)]
            if cur_box[0] > cur_box[2]:
                cur_box = [cur_box[2], cur_box[1], cur_box[0], cur_box[3]]
            if cur_box[1] > cur_box[3]:
                cur_box = [cur_box[0], cur_box[3], cur_box[2], cur_box[1]]
            boxes.append(cur_box)
            cur_score = uniform(0, 1)
            scores.append(cur_score)
        with open(dir + file_name + "_" + str(t) + ".txt", 'w') as file:
            file.write(str(boxes) + "\n" + str(scores))

def read_test(test_file):
    # print(test_file)
    with open(test_file, 'r') as file:
        lines = file.readlines()
        boxes = ast.literal_eval(lines[0])
        scores = ast.literal_eval(lines[1])
        return boxes, scores

def draw(boxes, scores, indeces=None):
    dir = "images/"
    Path(dir).mkdir(parents=True, exist_ok=True)
    if not indeces:
        indeces = range(len(boxes))
    img_3 = np.zeros([max_image_size, max_image_size, 3], dtype=np.uint8)
    img_3.fill(255)
    thickness = 1
    for i, box in enumerate(boxes):
        if i in indeces:
            color = (0, 255*(scores[i]), 255*(1-scores[i]))
            img_3 = cv2.rectangle(img_3, (int(box[0]),int(box[1])), (int(box[2]),int(box[3])), color, thickness)
    file_name = dir+"image_" + str(datetime.now()).replace(" ", "") + ".jpg"
    cv2.imwrite(file_name, img_3)