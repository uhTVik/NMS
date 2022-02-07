import glob
import torch
# print(torch.__version__)
import utilsNMS
from torchvision.ops import nms
from myNMS import my_nms, my_nms_opt
import time

iou_threshold = 0.5

def test_with_pytorch():
	# create dataset
	# utilsNMS.create_dataset_NMS(number_of_tests=10)

	# All files and directories ending with .txt and that don't begin with a dot:
	test_files = glob.glob("tests/*.txt")
	count = 0
	times = []
	for repeat in range(1):
		for test_file in test_files[0:]:
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

			if not (boxes_tensor[result].tolist() == resulted_boxes_me):
				print(boxes_tensor[result].tolist(), scores_tensor[result].tolist())
				print([[float(j) for j in i] for i in resulted_boxes_me], resulted_scores_me)
				count += 1
	print(str(count) + " failers")
	#
	print(len(times))
	print(sum(times)/len(times))


def test_with_my():
	# create dataset
	# utilsNMS.create_dataset_NMS(number_of_tests=10)

	# All files and directories ending with .txt and that don't begin with a dot:
	test_files = glob.glob("tests/*.txt")
	count = 0
	times = []
	for repeat in range(10):
		for test_file in test_files[0:]:
			boxes, scores = utilsNMS.read_test(test_file)

			t1 = time.time()
			resulted_boxes_me, resulted_scores_me = my_nms_opt(boxes, scores, iou_threshold)
			t2 = time.time()
			resulted_boxes_me_opt, resulted_scores_me_opt = my_nms(boxes, scores, iou_threshold)
			t3 = time.time()

			t_ix = (t2 - t1) - (t3 - t2)
			times.append(t_ix)

			if not (resulted_boxes_me_opt == resulted_boxes_me):
				print([[float(j) for j in i] for i in resulted_boxes_me])
				print([[float(j) for j in i] for i in resulted_boxes_me_opt])
				count += 1
	print(str(count) + " failers")
	#
	print(len(times))
	print(sum(times)/len(times))


def test_cpp_with_pytorch():
	# All files and directories ending with .txt and that don't begin with a dot:
	test_files = glob.glob("tests/*.txt")
	cpp_result_dir = "results/"
	count = 0
	for repeat in range(1):
		for test_file in test_files[0:]:
			boxes, scores = utilsNMS.read_test(test_file)


			boxes_tensor = torch.tensor(boxes, dtype=float)
			scores_tensor = torch.tensor(scores, dtype=float)

			result = nms(boxes_tensor, scores_tensor, iou_threshold)
			# read cpp results
			resulted_boxes_cpp, resulted_scores_cpp = utilsNMS.read_test(cpp_result_dir+test_file.split("/")[1])

			resulted_boxes_torch = boxes_tensor[result].tolist()
			resulted_scores_torch = scores_tensor[result].tolist()

			if not (boxes_tensor[result].tolist() == resulted_boxes_cpp):
				print(test_file)

				# print(boxes_tensor[result].tolist(), scores_tensor[result].tolist())
				# print([[float(j) for j in i] for i in resulted_boxes_cpp], resulted_scores_cpp)
				print(boxes_tensor[result].tolist())
				print([[float(j) for j in i] for i in resulted_boxes_cpp])
				count += 1
				# if test_file == "tests/test_2022-02-0417:14:33.739672_3148.txt":
				# 	break
			# else:
			# 	print(test_file)
	print(str(count) + " failers")


# test_with_pytorch()

# test_with_my()
test_cpp_with_pytorch()