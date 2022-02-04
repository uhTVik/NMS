import glob
import torch
# print(torch.__version__)
import utilsNMS
from torchvision.ops import nms
from myNMS import my_nms
import time

iou_threshold = 0.5

# create dataset
# utilsNMS.create_dataset_NMS(number_of_tests=10)

# All files and directories ending with .txt and that don't begin with a dot:
test_files = glob.glob("tests/*.txt")
count = 0
times = []
for repeat in range(1):
    for test_file in test_files[0:1000]:
        boxes, scores = utilsNMS.read_test(test_file)
        # print(boxes, scores, iou_threshold)
        # utilsNMS.draw(boxes, scores)

        boxes_tensor = torch.tensor(boxes, dtype=float)
        scores_tensor = torch.tensor(scores, dtype=float)

        t1 = time.time()
        result = nms(boxes_tensor, scores_tensor, iou_threshold)
        t2 = time.time()
        resulted_boxes_me, resulted_scores_me = my_nms(boxes, scores, iou_threshold)
        t3 = time.time()

        t_ix = (t2-t1)-(t3-t2)
        times.append(t_ix)

        resulted_boxes_torch = boxes_tensor[result].tolist()
        resulted_scores_torch = scores_tensor[result].tolist()

        # print(boxes_tensor)
        # print(boxes_tensor[result].tolist())
        # print(my_result)

        # utilsNMS.draw(resulted_boxes_torch, resulted_scores_torch)
        # utilsNMS.draw(resulted_boxes_me, resulted_scores_me)

        # print(boxes_tensor[result].tolist())
        # print([[float(j) for j in i] for i in resulted_boxes_me])

        if not boxes_tensor[result].tolist() == resulted_boxes_me:
            print(boxes_tensor[result].tolist())
            print([[float(j) for j in i] for i in resulted_boxes_me])
            count += 1
print(str(count) + " failers")
#
print(len(times))
print(sum(times))

