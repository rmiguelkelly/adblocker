adblocker:
	g++ -std=c++14 ./include/adblocker/*.cpp ./src/blocker.cpp -o blocker.o
clean:
	rm *.o