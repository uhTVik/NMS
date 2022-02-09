# NMS
Non-Maximum Suppression algorithm in python and C++.

This code compares speed of my C++ implementation
with my python implementation and pytorch implementation.

Python implementation is in the file python_src/my_NMS.py.
C++ implementation is in the file cpp_src/my_NMS.cpp.

### How to run

Before you run make sure you have python 3.7 and do the following: `poetry update`

If poetry does not work for you, just run `python_src/testNMS.py` with your python. 

1. `poetry run python python_src/testNMS.py --tests 10`

This process will generate 10 tests and compare implementations on these generated tests. 
2. `poetry run python python_src/testNMS.py --draw`

This process will draw boxes of previously generated tests and compare implementations on previously generated tests. 
3. `poetry run python python_src/testNMS.py -t 5 -d`

This process will generate 5 tests, draw boxes and compare implementations on these generated tests. 

4. `poetry run python python_src/testNMS.py`

This process will just compare implementations on previously generated tests. 

5. `make`

This process will just run C++ NMS on previously generated tests. 


#### Generated Folders

`tests/` -- folder for generated tests 

`results_cpp/` -- folder for C++ resulted boxes after C++ NMS.

`images/` -- folder for images of generated tests and their results after NMS

