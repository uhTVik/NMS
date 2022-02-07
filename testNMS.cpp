// basic file operations
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "myNMS.h"

const int numberOfCooords = 4;

void saveNMSTofile(std::string outputPath, std::string fileName, float **result, int numberOfBoxes, int numberOfCooords){
	create_directory(std::filesystem::path(outputPath));
	ofstream myfile(outputPath + fileName);
	int numberOfTargetBoxes = numberOfBoxes;
	for (int i = 0; i < numberOfBoxes; i++){
		bool zeros = true;
		for (int j = 0; j < numberOfCooords; j++){
			if (result[i][j] > 0.1){
				zeros = false;
			}
		}
		if (zeros){
			numberOfTargetBoxes = i;
			break;
		}
	}
//	cout << numberOfTargetBoxes << " ";
	myfile << "[";
	for (int i = 0; i < numberOfTargetBoxes; i++){
		myfile << "[";
		for (int j = 0; j < numberOfCooords; j++){
			if (j < numberOfCooords-1){
				myfile << result[i][j] << ", ";
			} else{
				myfile << result[i][j];
			}

		}
		if (i < numberOfTargetBoxes-1){
			myfile << "], ";
		}else{
			myfile << "]";
		}
	}
	myfile << "]";
	myfile << "\n[";
	for (int i = 0; i < numberOfTargetBoxes; i++){
		if (i < numberOfTargetBoxes-1){
			myfile << result[i][numberOfCooords] << ", ";
		}else{
			myfile << result[i][numberOfCooords];
		}
	}
	myfile << "]";
	myfile.close();
}

int getNumberOfBoxes(std::string strBoxes){
    char ch = '[';
 
    int count = -1;
    for (int i = 0; (i = strBoxes.find(ch, i)) != std::string::npos; i++) {
        count++;
    }

    return count;
}

void getBoxes(float **boxes, std::string strBoxes){
	stringstream ss;    
  
    /* Storing the whole string into string stream */
    ss << strBoxes;
  
    /* Running loop till the end of the stream */
    string temp;
    float found;
    int counter = 0;
    while (!ss.eof()) {
  
        /* extracting word by word from stream */
        ss >> temp;

        if (temp[1] == '['){
        	temp = temp.substr(2);
        }
        if (temp[0] == '['){
        	temp = temp.substr(1);
        }
  
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found){
            // cout << found << " ";
			// cout << counter/numberOfCooords << " ";
			// cout << counter%numberOfCooords<< " ";
        	boxes[counter/numberOfCooords][counter%numberOfCooords] = found;
  			// cout << boxes[counter/numberOfCooords][counter%numberOfCooords] << " ";
  			counter++;
  		}
        /* To save from space at the end of string */
        temp = "";
    }
}


void getScores(float *scores, std::string strScores){
	stringstream ss;    
  
    /* Storing the whole string into string stream */
    ss << strScores;
  
    /* Running loop till the end of the stream */
    string temp;
    float found;
    int counter = 0;
    while (!ss.eof()) {
  
        /* extracting word by word from stream */
        ss >> temp;

        if (temp[0] == '['){
        	temp = temp.substr(1);
        }
  
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found){
            // cout << found << " ";
			// cout << counter/numberOfCooords << " ";
			// cout << counter%numberOfCooords<< " ";
        	scores[counter] = found;
  			// cout << scores[counter] << " ";
  			counter++;
  			// cout << scores[counter] << " ";
  		}
        /* To save from space at the end of string */
        temp = "";
    }
}

int main (){
	std::string inputPath = "tests/";
	std::string outputPath = "results/";
	int i_of_tests = 0;
	int numberOfTests = -1;
	int numberOfBoxes = 0;
	for (const auto & entry : std::filesystem::directory_iterator(inputPath)){
		// std::cout << entry.path() << std::endl;
		std::ifstream myfile (entry.path()); // this is equivalent to the above method
//		std::ifstream myfile ("tests/test_2022-02-0417:14:33.739672_7448.txt");
		std::string strBoxes;
		std::string strScores;
		if (myfile.is_open()) {
			std::getline (myfile, strBoxes);
			if (numberOfBoxes == 0) {
				numberOfBoxes = getNumberOfBoxes(strBoxes);
				// cout << numberOfBoxes << "\n";
			}
			// float boxes[numberOfBoxes][numberOfCooords];
			float** boxes = new float*[numberOfBoxes];                
		    for (int i = 0; i < numberOfBoxes; i++){
				boxes[i] = new float[numberOfCooords];
		    }
			getBoxes(boxes, strBoxes);
			// cout << strBoxes << "\n";
			std::getline (myfile, strScores);
			// float scores[numberOfBoxes];
			float* scores = new float[numberOfBoxes];
			getScores(scores, strScores);
			// cout << strScores << "\n";
		
			// float resultedBoxes[numberOfBoxes][numberOfCooords];
			// float resultedScores[numberOfBoxes];
			// memset(resultedBoxes, 0, sizeof(resultedBoxes));
			// memset(resultedScores, 0, sizeof(resultedScores));
			// DO NMS HERE
			float iouThreshold = 0.5;
			float** result = myNMS(boxes, scores, numberOfBoxes, numberOfCooords, iouThreshold);
//			cout << "\n";
//			if (entry.path() == "tests/test_2022-02-0417:14:33.739672_4141.txt"){
//                 for (int i = 0; i < numberOfBoxes; i++){
//                    for (int j = 0; j < numberOfCooords+1; j++){
//                        cout << result[i][j] << " ";
//                    }
//                    cout << "\n";
//                 }
//                 break;
//             }
//             break;
			saveNMSTofile(outputPath, entry.path().filename(), result, numberOfBoxes, numberOfCooords);
//			break;
		}
		myfile.close();
		i_of_tests++;
		if (i_of_tests >= numberOfTests){
			if (numberOfTests != -1){
		   		break;
		   	}
	    }
	}
	return 0;
}