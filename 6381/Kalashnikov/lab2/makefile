KEYS=-Wall -Wextra

all: Greed.exe AStar.exe
	
AStar.exe:
	g++ AStar.o -g AStar.exe $(KEYS)
Greed.exe:
	g++ Greed.o -g Greed.exe $(KEYS)
Greed.o: Greed.cpp
	g++ -c Greed.cpp -o Greed.o
AStar.o: AStar.cpp
	g++ -c AStar.cpp -o AStar.o
