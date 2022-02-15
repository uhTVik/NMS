def merge_sort_for_NMS(scores, boxes):
	if len(scores) > 1:

		# Finding the mid of the array
		mid = len(scores) // 2

		# Dividing the array elements
		L_scores = scores[:mid]
		L_boxes = boxes[:mid]

		# into 2 halves
		R_scores = scores[mid:]
		R_boxes = boxes[mid:]

		# Sorting the first half
		merge_sort_for_NMS(L_scores, L_boxes)

		# Sorting the second half
		merge_sort_for_NMS(R_scores, R_boxes)

		i = j = k = 0

		# Copy data to temp arrays L[] and R[]
		while i < len(L_scores) and j < len(R_scores):
			if L_scores[i] > R_scores[j]:
				scores[k] = L_scores[i]
				boxes[k] = L_boxes[i]
				i += 1
			else:
				scores[k] = R_scores[j]
				boxes[k] = R_boxes[j]
				j += 1
			k += 1

		# Checking if any element was left
		while i < len(L_scores):
			scores[k] = L_scores[i]
			boxes[k] = L_boxes[i]
			i += 1
			k += 1

		while j < len(R_scores):
			scores[k] = R_scores[j]
			boxes[k] = R_boxes[j]
			j += 1
			k += 1

def insertion_sort_for_NMS(scores, boxes):
	# Start on the second element as we assume the first element is sorted
	n = len(scores)
	for i in range(1, n):
		score_to_insert = scores[i]
		box_to_insert = boxes[i]
		# And keep a reference of the index of the previous element
		j = i - 1
		# Move all items of the sorted segment forward if they are larger than
		# the item to insert
		while j >= 0 and scores[j] < score_to_insert:
			scores[j + 1] = scores[j]
			boxes[j + 1] = boxes[j]
			j -= 1
		# Insert the item
		scores[j + 1] = score_to_insert
		boxes[j + 1] = box_to_insert
	return boxes, scores


def sort_for_NMS(sort_algorithm, scores, boxes):
	if sort_algorithm == "insertion":
		# insertion sort for small N of boxes
		sorted_boxes, sorted_scores = insertion_sort_for_NMS(scores, boxes)

	if sort_algorithm == "merge":
		# merge sort for big N of boxes
		merge_sort_for_NMS(scores, boxes)
		sorted_boxes, sorted_scores = boxes, scores

	return sorted_boxes, sorted_scores


def my_nms(boxes, scores, iou_threshold=0.5):
	# Return an empty list, if no boxes given

	if len(boxes) == 0:
		return []

	# CHOOSE sorting algorithm: 'insertion' for small N boxes or 'merge' for big N boxes
	sort_algorithms = ["insertion", "merge"]
	sorted_boxes, sorted_scores = sort_for_NMS(sort_algorithms[0], scores, boxes)

	# Compute the area of the bounding boxes
	areas = []
	for b in sorted_boxes:
		cur_area = (b[2] - b[0])*(b[3] - b[1])
		areas.append(cur_area)

	n = len(sorted_boxes)
	indeces_to_remain = list(range(0, n))
	for i, b_i in enumerate(sorted_boxes):
		if i not in indeces_to_remain:
			continue
		for j, b_j in enumerate(sorted_boxes):
			if j not in indeces_to_remain:
				continue
			if i == j:
				continue
			if areas[j] == 0:
				continue
			inter = max(min(b_i[2], b_j[2])-max(b_i[0], b_j[0]), 0)*max(min(b_i[3], b_j[3])-max(b_i[1], b_j[1]), 0)
			ion = inter/(areas[i] + areas[j] - inter)
			if ion > iou_threshold:
				indeces_to_remain.remove(j)

	boxes_to_remain = []
	scores_to_remain = []
	for i in indeces_to_remain:
		boxes_to_remain.append(sorted_boxes[i])
		scores_to_remain.append(sorted_scores[i])

	return boxes_to_remain, scores_to_remain


def my_nms_opt(boxes, scores, iou_threshold=0.5):
	# Return an empty list, if no boxes given

	if len(boxes) == 0:
		return []

	sorted_boxes, sorted_scores = insertion_sort_for_NMS(scores, boxes)


	# Compute the area of the bounding boxes
	areas = []
	for b in sorted_boxes:
		cur_area = (b[2] - b[0]) * (b[3] - b[1])
		areas.append(cur_area)

	n = len(sorted_boxes)
	indeces_to_remain = list(range(0, n))
	ions = {}
	for i, b_i in enumerate(sorted_boxes):
		if i not in indeces_to_remain:
			continue
		for j, b_j in enumerate(sorted_boxes):
			if j not in indeces_to_remain:
				continue
			if i == j:
				continue
			if areas[j] == 0:
				continue
			ion = 0
			if (j, i) in ions:
				ion = ions[(j, i)]
			else:
				inter = max(min(b_i[2], b_j[2]) - max(b_i[0], b_j[0]), 0) * max(min(b_i[3], b_j[3]) - max(b_i[1], b_j[1]),0)
				ion = inter / (areas[i] + areas[j] - inter)
				ions[(i, j)] = ion
			if ion > iou_threshold:
				indeces_to_remain.remove(j)

	boxes_to_remain = []
	scores_to_remain = []
	for i in indeces_to_remain:
		boxes_to_remain.append(sorted_boxes[i])
		scores_to_remain.append(sorted_scores[i])

	return boxes_to_remain, scores_to_remain