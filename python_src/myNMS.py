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

def my_nms(boxes, scores, iou_threshold=0.5):
	# Return an empty list, if no boxes given

	if len(boxes) == 0:
		return []

	sorted_boxes, sorted_scores = insertion_sort_for_NMS(scores, boxes)

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