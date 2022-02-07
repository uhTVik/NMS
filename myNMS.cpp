#include "myNMS.h"
#include <iostream>
using namespace std;

float maxForNMS(float a, float b){
	if (a >= b) {
		return a;
	}
	return b;
}
float minForNMS(float a, float b){
	if (a <= b) {
		return a;
	}
	return b;
}

bool isDuplicate(int i, int *indecesToRemain, int numberOfBoxes){
	bool duplicate = true;
	for (int k = 0; k < numberOfBoxes; k++){
		if (indecesToRemain[k] == i){
			duplicate = false;
		}
	}
	return duplicate;
}

void insertionSortForNMS(float **boxes, float *scores, int numberOfBoxes, int numberOfCooords){
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

float** myNMS(float **boxes, float *scores, int numberOfBoxes, int numberOfCooords,  float iouThreshold){ // float **resultedBoxesAndScores, float *resultedScores, 
	// cout << "YES";
	if (numberOfBoxes == 0){
		return nullptr;
	}

//	 for (int i = 0; i < numberOfBoxes; i++){
//	 	cout << scores[i] << " ";
//	 	for (int j = 0; j < numberOfCooords; j++){
//	 		cout << boxes[i][j] << " ";
//	 	}
//	 	cout << "\n";
//	 }
//	 cout << "\n";

	insertionSortForNMS(boxes, scores, numberOfBoxes, numberOfCooords);
	
//	 for (int i = 0; i < numberOfBoxes; i++){
//	 	cout << scores[i] << " ";
//	 	for (int j = 0; j < numberOfCooords; j++){
//	 		cout << boxes[i][j] << " ";
//	 	}
//	 	cout << "\n";
//	 }
//	 cout << "\n";

	// Compute the area of the bounding boxes
	int numberOfIndecesToRemain = numberOfBoxes;
	int *indecesToRemain = new int[numberOfBoxes];
	float* areas = new float[numberOfBoxes];

	for (int i = 0; i < numberOfBoxes; i++){
		indecesToRemain[i] = i;
		areas[i] = (boxes[i][2] - boxes[i][0])*(boxes[i][3] - boxes[i][1]);
		// cout << areas[i] << " ";
	}


	for (int i = 0; i < numberOfBoxes; i++){
		// check if box i is still not a duplicate 
		if (isDuplicate(i, indecesToRemain, numberOfBoxes)){
			continue;
		}
		for (int j = 0; j < numberOfBoxes; j++){
			if (i == j){
				continue;
			}
			if (areas[j] == 0){
				continue;
			}
			// check if box j is still not a duplicate 
			if (isDuplicate(j, indecesToRemain, numberOfBoxes)){
				continue;
			}
			float inter = maxForNMS(minForNMS(boxes[i][2], boxes[j][2])-maxForNMS(boxes[i][0], boxes[j][0]), 0)*maxForNMS(minForNMS(boxes[i][3], boxes[j][3])-maxForNMS(boxes[i][1], boxes[j][1]), 0);
			float ion = inter/(areas[i] + areas[j] - inter);
			if (ion > iouThreshold){
//			    if (boxes[j][0] == 68){
//			        cout << ion << " ";
//			    }
//			    cout << i << " "  << j << " " << boxes[j][0] << " " << boxes[j][1] << " " << ion << "\n";
				// indecesToRemain  remove j
				numberOfIndecesToRemain--;
				indecesToRemain[j] = -1;
			}
		}
	}
//    cout << numberOfIndecesToRemain << "\n";
    int counter = 0;
	float** resultedBoxesAndScores = new float*[numberOfBoxes];                
	for (int i = 0; i < numberOfBoxes; i++){
		resultedBoxesAndScores[i] = new float[numberOfCooords+1];
		for (int j = 0; j < numberOfCooords; j++){
			if (indecesToRemain[i] != -1){
				resultedBoxesAndScores[counter][j] = boxes[indecesToRemain[i]][j];
			}else{
				resultedBoxesAndScores[counter][j] = 0;
			}
		}
		if (indecesToRemain[i] != -1){
			resultedBoxesAndScores[counter][numberOfCooords] = scores[i];
			counter++;
		}else{
			resultedBoxesAndScores[counter][numberOfCooords] = 0;
		}
	}
	return resultedBoxesAndScores;
}