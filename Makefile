INCPATH = -I/usr/local/include/opencv
LIBPATH = -L/usr/local/bin
OPTIONS = -lhighgui -lstdc++
RM = /bin/rm

all: vision

clean:
	$(RM) -f *.o *.out vision
	cd ./gui && $(RM) -f *.o *.out
	cd ./imagelib && $(RM) -f *.o *.out

### LINK ##########
vision: vision.o imagelib/dim.o imagelib/hough.o imagelib/overlay.o \
		imagelib/sepchannel.o gui/Trackbar.o
	g++ -g vision.o imagelib/dim.o imagelib/hough.o imagelib/overlay.o \
		imagelib/sepchannel.o gui/Trackbar.o $(LIBPATH) $(OPTIONS) -o vision

### COMPILE #######
vision.o: vision.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c vision.cpp

imagelib/dim.o: imagelib/dim.cpp imagelib/dim.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c dim.cpp

imagelib/hough.o: imagelib/hough.cpp imagelib/hough.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c hough.cpp

imagelib/overlay.o: imagelib/overlay.cpp imagelib/overlay.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c overlay.cpp

imagelib/sepchannel.o: imagelib/sepchannel.cpp imagelib/sepchannel.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c sepchannel.cpp

gui/Trackbar.o: gui/Trackbar.cpp gui/Trackbar.hpp
	cd ./gui && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Trackbar.cpp

