all:
	g++ --std=c++17 testNMS.cpp myNMS.cpp -o testNMS.out
	./testNMS.out
