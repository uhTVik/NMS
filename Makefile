all:
	g++ --std=c++17 cpp_src/testNMS.cpp cpp_src/myNMS.cpp martinK_cpp_src/nms.cpp -o cpp_src/testNMS.out
	./cpp_src/testNMS.out