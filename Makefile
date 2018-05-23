all: lr4-1 lr4-2
lr4-1: lr4-1.o
	g++ -std=c++11 lr4-1.o -o lr4-1 
lr4-2: lr4-2.o
	g++ -std=c++11 lr4-2.o -o lr4-2
lr4-1.o: lr4-1.cpp
	g++ -std=c++11 -c lr4-1.cpp
lr4-2.o: lr4-2.cpp
	g++ -std=c++11 -c lr4-2.cpp
 