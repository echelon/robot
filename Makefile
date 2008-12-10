INCPATH = -I/usr/local/include/opencv
LIBPATH = -L/usr/local/bin
OPTIONS = -lhighgui -lstdc++

all: vision

clean:
	/bin/rm -R -f vision *.o *.out

### LINK ##########
vision: vision.o transform/hough.o gui/Trackbar.o
	g++ -g vision.o transform/hough.o gui/Trackbar.o $(LIBPATH) $(OPTIONS) -o vision

### COMPILE #######
vision.o: vision.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c vision.cpp

transform/hough.o: transform/hough.o
	cd ./transform && g++ -g -Wall $(INCPATH) $(OPTIONS) -c hough.cpp

gui/Trackbar.o: gui/Trackbar.cpp gui/Trackbar.hpp
	cd ./gui && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Trackbar.cpp

