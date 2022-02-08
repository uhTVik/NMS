import glob
import torch
import utilsNMS
from torchvision.ops import nms
from myNMS import my_nms, my_nms_opt
import time
import subprocess
from argparse import ArgumentParser

# be aware to change iou_threshold for cpp implementation to get 0 failers
iou_threshold = 0.5

# TODO: questions:
# 1. which input/output? -> now -- good
# 2. validate boxes??? or anything else? -> not necessary
# 3. 9 boxes, how can we be sure? 9-15 boxes: 1:1, 1:2, 2:1
# 4. C version? -> no
# 5. Is current result ok?
# 6. More tests? Which?
# 7. CUDA parallelize? not necessary

# 3 000 000 boxes

def main_test(number_of_tests=0, draw=False):
	# create dataset if number of tests more than 0
	if number_of_tests > 0:
		utilsNMS.create_dataset_NMS(number_of_tests)
	# run cpp code
	make_process = subprocess.call("make", stderr=subprocess.STDOUT)
	# All files and directories ending with .txt and that don't begin with a dot:
	test_files = glob.glob("tests/*.txt")
	cpp_result_dir = "results_cpp/"
	failers_count_me = 0
	failers_count_cpp = 0
	pytorchNMS_time = 0
	myNMS_time = 0
	all_counter = 0
	for repeat in range(1):
		for test_file in test_files[0:100]:
			all_counter += 1
			boxes, scores = utilsNMS.read_test(test_file)
			if draw:
				utilsNMS.draw(test_file, "before", boxes, scores)

			boxes_tensor = torch.tensor(boxes, dtype=float)
			scores_tensor = torch.tensor(scores, dtype=float)

			t1 = time.time_ns()
			result = nms(boxes_tensor, scores_tensor, iou_threshold)
			t2 = time.time_ns()
			resulted_boxes_me, resulted_scores_me = my_nms(boxes, scores, iou_threshold)
			t3 = time.time_ns()

			# time calculation
			pytorchNMS_time = pytorchNMS_time + (t2-t1)
			myNMS_time = myNMS_time + (t3-t2)
			# read cpp results
			resulted_boxes_cpp, resulted_scores_cpp = utilsNMS.read_test(cpp_result_dir+test_file.split("/")[1])

			if not (boxes_tensor[result].tolist() == resulted_boxes_me):
				print("my python fail")
				print(test_file)
				print(boxes_tensor[result].tolist(), scores_tensor[result].tolist())
				print([[float(j) for j in i] for i in resulted_boxes_me], resulted_scores_me)
				failers_count_me += 1

			if not (boxes_tensor[result].tolist() == resulted_boxes_cpp):
				print("my cpp fail")
				print(test_file)
				print(boxes_tensor[result].tolist())
				print([[float(j) for j in i] for i in resulted_boxes_cpp])
				failers_count_cpp += 1
			if draw:
				utilsNMS.draw(test_file, "after", boxes_tensor[result].tolist(), scores_tensor[result].tolist())

	print("myNMS time: " + str(int(myNMS_time/all_counter)) + " ns")
	print("pytorchNMS time: " + str(int(pytorchNMS_time/all_counter)) + " ns")
	print(str(failers_count_cpp) + " cpp failers")
	print(str(failers_count_me) + " python failers")

if __name__ == "__main__":
	parser = ArgumentParser()
	parser.add_argument("-t", "--tests", dest="number_of_tests", default=0, type=int,
						help="number of tests to generate")
	parser.add_argument("-d", "--draw", dest="draw", default=False, action="store_true",
						help="draw tests")

	args = parser.parse_args()
	main_test(args.number_of_tests, args.draw)