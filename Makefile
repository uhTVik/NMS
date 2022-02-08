all:
	g++ --std=c++17 cpp_src/testNMS.cpp cpp_src/myNMS.cpp -o cpp_src/testNMS.out
	./cpp_src/testNMS.out

c:
	gcc c_src/testNMS.c c_src/myNMS.c -o c_src/testNMS.out
	./c_src/testNMS.out
