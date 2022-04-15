// basic file operations
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

using namespace std;


#include <vector>

#include "myNMS.h"
#include "../martinK_cpp_src/nms.hpp"

const int numberOfCoords = 4;

// save NMS result to file
void saveNMSTofile(std::string outputPath, std::string fileName, float **result, int numberOfBoxes, int numberOfCoords){
	// creat directory where the result is stored
	fs::create_directory(fs::path(outputPath));
	// set output stream
	ofstream myfile(outputPath + fileName);

	// find the number of target boxes, check when the array starts to have zeros
	int numberOfTargetBoxes = numberOfBoxes; // init with numberOfBoxes, because there can be a case when there are no zeros
	for (int i = 0; i < numberOfBoxes; i++){
		bool zeros = true;
		for (int j = 0; j < numberOfCoords+1; j++){
			if (result[i][j] != 0){
				zeros = false;
			}
		}
		if (zeros){
			numberOfTargetBoxes = i;
			break;
		}
	}

	// start writing to a file
	myfile << "["; // open list of boxes
	for (int i = 0; i < numberOfTargetBoxes; i++){
	    // start box info
		myfile << "[";
		// fill box info
		for (int j = 0; j < numberOfCoords; j++){
			if (j < numberOfCoords-1){
				myfile << result[i][j] << ", ";
			} else{
				myfile << result[i][j];
			}

		}
		// end box info
		if (i < numberOfTargetBoxes-1){
			myfile << "], ";
		}else{
			myfile << "]";
		}
	}
	myfile << "]";// close list of boxes
	myfile << "\n[";// open list of scores
    // fill scores info
	for (int i = 0; i < numberOfTargetBoxes; i++){
		if (i < numberOfTargetBoxes-1){
			myfile << result[i][numberOfCoords] << ", ";
		}else{
			myfile << result[i][numberOfCoords];
		}
	}
	myfile << "]"; // close list of scores
	myfile.close(); // close file
}


// find number of '[' in the line of boxes -1
int getNumberOfBoxes(std::string strBoxes){
    char ch = '[';
 
    int count = -1;
    for (int i = 0; (i = strBoxes.find(ch, i)) != std::string::npos; i++) {
        count++;
    }

    return count;
}

// read info about boxes
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

        // ignore "[" in reading numbers
        if (temp[1] == '['){
        	temp = temp.substr(2);
        }
        if (temp[0] == '['){
        	temp = temp.substr(1);
        }
  
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found){
        	boxes[counter/numberOfCoords][counter%numberOfCoords] = found;
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

        // ignore "[" in reading numbers
        if (temp[0] == '['){
        	temp = temp.substr(1);
        }
  
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found){
        	scores[counter] = found;
  			counter++;
  		}
        /* To save from space at the end of string */
        temp = "";
    }
}

std::vector<std::vector<float>> toVector(float **boxes, float *scores, int N, int M){
    std::vector<std::vector<float>> vals = {};

    for (int i = 0; i < N; i++){
        std::vector<float> currentVals = {};
        for ( int j = 0; j < M; j++){
            currentVals.push_back(boxes[i][j]);
        }
        currentVals.push_back(scores[i]);
        vals.push_back(currentVals);
    }

  	return vals;
}

float** toArray(std::vector<std::vector<float> > &vals, int N, int M){
//    for (int i = 0; i < vals.size(); i++)
//    {
//        for (int j = 0; j < vals[i].size(); j++)
//        {
//            cout << vals[i][j] << " ";
//        }
//        cout << "\n";
//    }
   float** arr = new float*[N];
   for (int i = 0; i < N; i++){
        arr[i] = new float[M];
        if (i < vals.size()) {
            for (int j = 0; j < M; j++){
                arr[i][j] = vals[i][j];
            }
        } else {
            for (int j = 0; j < M; j++){
                arr[i][j] = 0;
            }
        }
   }
   return arr;
 }

// main process to get the result from c++ NMS implementation
int main (){

    float iouThreshold = 0.5;

//    std::vector<std::vector<float>> rectangles =
//    {
//    {300, 300, 400, 400},
//    {320, 320, 420, 420},
//    {295, 259, 415, 415},
//    {100, 100, 150, 150},
//    {90,  90,  180, 180},
//    {112, 112, 170, 170}
//    };

    // set iouThreshold for NMS

    // folders info
	std::string inputPath = "tests/";
	std::string outputPath = "results_cpp/";

	for (int haha = 0; haha < 1; haha++){

        int i_of_tests = 0;

        // number of tests. '-1' means "use all tests"
        int numberOfTests = -1;
        // number of boxes. '-1' means we do not know the number of boxes yet
        int numberOfBoxes = -1;
        long allTime = 0;
        for (const auto & entry : fs::directory_iterator(inputPath)){
            // start reading file
            std::ifstream myfile(entry.path()); // this is equivalent to the above method
    //		std::ifstream myfile("tests/test_2022-02-0417:14:33.739672_7448.txt");
//            std::cout << entry.path() << "\n";
            // strings for boxes and scores
            std::string strBoxes;
            std::string strScores;

            // read current file
            if (myfile.is_open()) {
//                std::cout << i_of_tests << " ";
                // get first line with boxes
                std::getline(myfile, strBoxes);
                // if we do not know the number of boxes, we get it from the read line
                if (numberOfBoxes == -1) {
                    numberOfBoxes = getNumberOfBoxes(strBoxes);
                }
                // init the array for boxes
                float** boxes = new float*[numberOfBoxes];
                for (int i = 0; i < numberOfBoxes; i++){
                    boxes[i] = new float[numberOfCoords];
                }
                // get boxes info
                getBoxes(boxes, strBoxes);

                // get second line with scores
                std::getline (myfile, strScores);
                // init the array for scores
                float* scores = new float[numberOfBoxes];
                // get scores info
                getScores(scores, strScores);
//                std::vector<std::vector<float>> rectangles = toVector(boxes, scores, numberOfBoxes, numberOfCoords);
                // perform NMS
                auto started = std::chrono::high_resolution_clock::now();
    //			float** result = myNMS(boxes, scores, numberOfBoxes, numberOfCoords, iouThreshold, false);
//    			float** result = myNMS(boxes, scores, numberOfBoxes, numberOfCoords, iouThreshold);
//                std::vector<std::vector<float>> reducedRectangle = nms(rectangles, iouThreshold);
                float** result = myNMSwithVector(boxes, scores, numberOfBoxes, numberOfCoords, iouThreshold);
                auto done = std::chrono::high_resolution_clock::now();
//                float** result = toArray(reducedRectangle, numberOfBoxes, numberOfCoords+1);
    //			cout << std::chrono::duration_cast<std::chrono::nanoseconds>(done-started).count() << "\n";
                allTime = allTime + std::chrono::duration_cast<std::chrono::microseconds>(done-started).count();

                // save result to corresponding file
                saveNMSTofile(outputPath, entry.path().filename(), result, numberOfBoxes, numberOfCoords);

                // printing for debugging
    //			cout << "\n";
    //			if (entry.path() == "tests/test_2022-02-0417:14:33.739672_4141.txt"){
    //                 for (int i = 0; i < numberOfBoxes; i++){
    //                    for (int j = 0; j < numberOfCoords+1; j++){
    //                        cout << result[i][j] << " ";
    //                    }
    //                    cout << "\n";
    //                 }
    //                 break;
    //            }
    //            break;
            }
            // close file
            myfile.close();

            // stop when we exceed the number of tests thar we have to process
            i_of_tests++;
            if (i_of_tests >= numberOfTests){
                if (numberOfTests != -1){
                    break;
                }
            }
        }
    //	cout << allTime << "\n";
    //	cout << i_of_tests << "\n";
        cout << "cppNMS time: " << (1000*allTime)/i_of_tests << " ns\n";
    }
	return 0;
}
