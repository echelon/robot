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
vision: vision.o imagelib/colordim.o imagelib/colorreplace.o imagelib/hough.o \
		imagelib/overlay.o imagelib/sepchannel.o gui/Trackbar.o \
		serial/serializer.o tts/Festival.o
	g++ -g vision.o imagelib/colordim.o imagelib/colorreplace.o imagelib/hough.o \
		imagelib/overlay.o imagelib/sepchannel.o gui/Trackbar.o \
		serial/serializer.o tts/Festival.o $(LIBPATH) $(OPTIONS) -o vision

### COMPILE #######
vision.o: vision.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c vision.cpp

imagelib/colordim.o: imagelib/colordim.cpp imagelib/colordim.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c colordim.cpp

imagelib/colorreplace.o: imagelib/colorreplace.cpp imagelib/colorreplace.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c colorreplace.cpp

imagelib/hough.o: imagelib/hough.cpp imagelib/hough.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c hough.cpp

imagelib/overlay.o: imagelib/overlay.cpp imagelib/overlay.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c overlay.cpp

imagelib/sepchannel.o: imagelib/sepchannel.cpp imagelib/sepchannel.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c sepchannel.cpp

serial/serializer.o: serial/serializer.cpp serial/serializer.hpp
	cd ./serial && g++ -g -Wall $(INCPATH) $(OPTIONS) -c serializer.cpp

gui/Trackbar.o: gui/Trackbar.cpp gui/Trackbar.hpp
	cd ./gui && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Trackbar.cpp

tts/Festival.o: tts/Festival.cpp tts/Festival.hpp
	cd ./tts && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Festival.cpp

