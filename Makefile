INCPATH = -I/usr/local/include/opencv
LIBPATH = -L/usr/local/bin
OPTIONS = -lhighgui -lstdc++
RM = /bin/rm

all: vision

clean:
	$(RM) -f vision *.o *.out
	cd ./gui && $(RM) -f *.o *.out
	cd ./libs && $(RM) -f *.o *.out
	cd ./transform && $(RM) -f *.o *.out

### LINK ##########
vision: vision.o transform/hough.o transform/overlay.o gui/Trackbar.o
	g++ -g vision.o transform/hough.o \
		transform/overlay.o gui/Trackbar.o $(LIBPATH) $(OPTIONS) -o vision

### COMPILE #######
vision.o: vision.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c vision.cpp

transform/hough.o: transform/hough.cpp transform/hough.hpp
	cd ./transform && g++ -g -Wall $(INCPATH) $(OPTIONS) -c hough.cpp

transform/overlay.o: transform/overlay.cpp transform/overlay.hpp
	cd ./transform && g++ -g -Wall $(INCPATH) $(OPTIONS) -c overlay.cpp

gui/Trackbar.o: gui/Trackbar.cpp gui/Trackbar.hpp
	cd ./gui && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Trackbar.cpp

