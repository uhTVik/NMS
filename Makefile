all:
	g++ --std=c++17 cpp_src/testNMS.cpp cpp_src/myNMS.cpp -o cpp_src/testNMS.out
	./cpp_src/testNMS.out