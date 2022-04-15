from datetime import datetime
import ast
from pathlib import Path
import numpy as np
import cv2
from random import randint, uniform, gauss

import shutil



max_image_size = 1500

# boxes are as follows: [x1, y1, x2, y2], x2 > x1, y2 > y1
def create_dataset_NMS(number_of_tests=10, number_of_candidates=9):
    sigma = 1
    min_abs_side = 30

    dir = "tests/"

    shutil.rmtree(dir, ignore_errors=True)

    Path(dir).mkdir(parents=True, exist_ok=True)
    file_name = "test_" + str(datetime.now()).replace(" ", "")
    for t in range(number_of_tests):
        boxes = []
        scores = []
        types = [(0.5, 1, 0.25, 0), (1, 0.5, 0, 0.25), (1, 1, 0, 0)]

        target_box = [randint(0, max_image_size-1), randint(0, max_image_size-1), randint(0, max_image_size-1),
                      randint(0, max_image_size-1)]
        if target_box[0] > target_box[2]:
            target_box = [target_box[2], target_box[1], target_box[0], target_box[3]]
        if target_box[1] > target_box[3]:
            target_box = [target_box[0], target_box[3], target_box[2], target_box[1]]
        for i in range(number_of_candidates):

            min_side_target = min(target_box[2] - target_box[0], target_box[3] - target_box[1])
            cur_box = [min(max(target_box[0] + round(gauss(0, sigma*min_side_target**0.5)), 0), max_image_size-1),
                       min(max(target_box[1] + round(gauss(0, sigma*min_side_target**0.5)), 0), max_image_size-1),
                       min(max(min_abs_side+target_box[2] + round(gauss(0, sigma*min_side_target**0.5)), 0), max_image_size-1),
                       min(max(min_abs_side+target_box[3] + round(gauss(0, sigma*min_side_target**0.5)), 0), max_image_size-1)]
            if cur_box[0] > cur_box[2]:
                cur_box = [cur_box[2], cur_box[1], cur_box[0], cur_box[3]]
            if cur_box[1] > cur_box[3]:
                cur_box = [cur_box[0], cur_box[3], cur_box[2], cur_box[1]]

            cur_type = types[randint(0, 2)]
            min_side = min(cur_box[2]-cur_box[0], cur_box[3]-cur_box[1])

            cur_box = [cur_box[0]+round(min_side*cur_type[2]), cur_box[1]+round(min_side*cur_type[3]), cur_box[0]+round(min_side*cur_type[0])+round(min_side*cur_type[2]), cur_box[1]+round(min_side*cur_type[1])+round(min_side*cur_type[3])]
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

def draw(test_file, prefix, boxes, scores, indeces=None):
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
    file_name = dir+"image_" + test_file.split("/")[-1][0:-4] + "_" + prefix + ".jpg"
    cv2.imwrite(file_name, img_3)