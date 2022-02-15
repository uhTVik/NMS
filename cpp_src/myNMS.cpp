#include "myNMS.h"
//#include <iostream>
//using namespace std;


// return max of 2 float numbers
float maxForNMS(float a, float b){
	if (a >= b) {
		return a;
	}
	return b;
}

// return min of 2 float numbers
float minForNMS(float a, float b){
	if (a <= b) {
		return a;
	}
	return b;
}

// check if current index is already a duplicate within the boxes passed to NMS
bool isDuplicate(int i, int *indicesToRemain, int numberOfBoxes){
	bool duplicate = true;
	for (int k = 0; k < numberOfBoxes; k++){
		if (indicesToRemain[k] == i){
			duplicate = false;
		}
	}
	return duplicate;
}

// insertion sort for NMS algorithm
// sort boxes and scores by score value
void insertionSortForNMS(float **boxes, float *scores, int numberOfBoxes, int numberOfCoords){
    int i, j;
    float cur_score;
    float *cur_box;
    for (i = 1; i < numberOfBoxes; i++)
    {
        cur_score = scores[i];
        cur_box = boxes[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
        greater than cur_score, to one position ahead
        of their current position */
        while (j >= 0 && scores[j] < cur_score)
        {
            scores[j + 1] = scores[j];
            boxes[j + 1] = boxes[j];
            j = j - 1;
        }
        scores[j + 1] = cur_score;
        boxes[j + 1] = cur_box;
    }
}
void merge(float **boxes, float *scores, int const left, int const mid, int const right, int numberOfCoords)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    // Create temp arrays
    float *leftArrayScores = new float[subArrayOne];
    float *rightArrayScores = new float[subArrayTwo];
    float **leftArrayBoxes = new float*[subArrayOne];
    float **rightArrayBoxes = new float*[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (int i = 0; i < subArrayOne; i++){
        leftArrayScores[i] = scores[left + i];
    }
    for (int i = 0; i < subArrayTwo; i++) {
        rightArrayScores[i] = scores[mid + 1 + i];
    }

    for (int i = 0; i < subArrayOne; i++){
        leftArrayBoxes[i] = new float[numberOfCoords];
        for (int j = 0; j < numberOfCoords; j++){
            leftArrayBoxes[i][j] = boxes[left + i][j];
        }
    }
    for (int i = 0; i < subArrayTwo; i++) {
        rightArrayBoxes[i] = new float[numberOfCoords];
        for (int j = 0; j < numberOfCoords; j++){
            rightArrayBoxes[i][j] = boxes[mid + 1 + i][j];
        }
    }

    int indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArrayScores[indexOfSubArrayOne] >= rightArrayScores[indexOfSubArrayTwo]) {
            scores[indexOfMergedArray] = leftArrayScores[indexOfSubArrayOne];
            for (int j = 0; j < numberOfCoords; j++){
                boxes[indexOfMergedArray][j] = leftArrayBoxes[indexOfSubArrayOne][j];
            }
            indexOfSubArrayOne++;
        }
        else {
            scores[indexOfMergedArray] = rightArrayScores[indexOfSubArrayTwo];
            for (int j = 0; j < numberOfCoords; j++){
                boxes[indexOfMergedArray][j] = rightArrayBoxes[indexOfSubArrayTwo][j];
            }
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        scores[indexOfMergedArray] = leftArrayScores[indexOfSubArrayOne];
        for (int j = 0; j < numberOfCoords; j++){
            boxes[indexOfMergedArray][j] = leftArrayBoxes[indexOfSubArrayOne][j];
        }
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        scores[indexOfMergedArray] = rightArrayScores[indexOfSubArrayTwo];
        for (int j = 0; j < numberOfCoords; j++){
            boxes[indexOfMergedArray][j] = rightArrayBoxes[indexOfSubArrayTwo][j];
        }
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSortForNMS(float **boxes, float *scores, int const begin, int const end, int numberOfCoords)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSortForNMS(boxes, scores, begin, mid, numberOfCoords);
    mergeSortForNMS(boxes, scores, mid + 1, end, numberOfCoords);
    merge(boxes, scores, begin, mid, end, numberOfCoords);
}

// NMS algorithm
// boxes are as follows: [x1, y1, x2, y2], x2 > x1, y2 > y1
float** myNMS(float **boxes, float *scores, int numberOfBoxes, int numberOfCoords,  float iouThreshold, bool merge){
    // TODO: validate boxes??? or anything else?
    // check if we have boxes
//	if (numberOfBoxes == 0){
//		return nullptr;
//	}

    // print boxes before insertion sort
//	 for (int i = 0; i < numberOfBoxes; i++){
//	 	cout << scores[i] << " ";
//	 	for (int j = 0; j < numberOfCoords; j++){
//	 		cout << boxes[i][j] << " ";
//	 	}
//	 	cout << "\n";
//	 }
//	 cout << "\n";

    // sort boxes and scores by score value
//	insertionSortForNMS(boxes, scores, numberOfBoxes, numberOfCoords);
    if (merge){
    	mergeSortForNMS(boxes, scores, 0, numberOfBoxes-1, numberOfCoords);
    }else{
        insertionSortForNMS(boxes, scores, numberOfBoxes, numberOfCoords);
    }

	// print boxes after insertion or merge sort
//	 for (int i = 0; i < numberOfBoxes; i++){
//	 	cout << scores[i] << " ";
//	 	for (int j = 0; j < numberOfCoords; j++){
//	 		cout << boxes[i][j] << " ";
//	 	}
//	 	cout << "\n";
//	 }
//	 cout << "\n";

    // variables for saving boxes that have to be remained
    int numberOfIndicesToRemain = numberOfBoxes;
    int *indicesToRemain = new int[numberOfBoxes];

	// compute the areas of the bounding boxes
	float* areas = new float[numberOfBoxes];
	for (int i = 0; i < numberOfBoxes; i++){
	    // fill array of indices to remain
		indicesToRemain[i] = i;
		// compute area of each box: (x2-x1)*(y2-y1)
		areas[i] = (boxes[i][2] - boxes[i][0])*(boxes[i][3] - boxes[i][1]);
	}


	for (int i = 0; i < numberOfBoxes; i++){
		// check if box i is still not a duplicate 
		if (isDuplicate(i, indicesToRemain, numberOfBoxes)){
			continue;
		}
		for (int j = 0; j < numberOfBoxes; j++){
		    // do not compare same box
			if (i == j){
				continue;
			}
			// if area of the box is 0, we will not discard it
			if (areas[j] == 0){
				continue;
			}
			// check if box j is still not a duplicate 
			if (isDuplicate(j, indicesToRemain, numberOfBoxes)){
				continue;
			}
			// calculate intersection between boxes i and j
			float inter = maxForNMS(minForNMS(boxes[i][2], boxes[j][2])-maxForNMS(boxes[i][0], boxes[j][0]), 0)*maxForNMS(minForNMS(boxes[i][3], boxes[j][3])-maxForNMS(boxes[i][1], boxes[j][1]), 0);
			// calculate intersection over union (ion) between boxes i and j
			float ion = inter/(areas[i] + areas[j] - inter);
			// check if ion is larger then the threshold abd remove box j if it is true
			if (ion > iouThreshold){
				// indicesToRemain  remove j
				numberOfIndicesToRemain--;
				indicesToRemain[j] = -1;
			}
		}
	}
	// counter for the target boxes
    int counter = 0;
    // 2d array for target boxes and their scores: [x1, y1, x2, y2, score]
	float** targetBoxesAndScores = new float*[numberOfBoxes];
	for (int i = 0; i < numberOfBoxes; i++){
		targetBoxesAndScores[i] = new float[numberOfCoords+1]; // numberOfCoords + 1 for the score
		// fill boxes coordinates
		for (int j = 0; j < numberOfCoords; j++){
			if (indicesToRemain[i] != -1){
				targetBoxesAndScores[counter][j] = boxes[indicesToRemain[i]][j];
			}
		}
		// fill boxes scores
		if (indicesToRemain[i] != -1){
			targetBoxesAndScores[counter][numberOfCoords] = scores[i];
			counter++;
		}
	}
	// fill remaining array with zeros
    for (int i = counter; i < numberOfBoxes; i++){
		for (int j = 0; j < numberOfCoords + 1; j++){
		    targetBoxesAndScores[counter][j] = 0;
		}
    }
	return targetBoxesAndScores;
}