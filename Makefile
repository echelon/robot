### VARIABLES #############
COMPILE = g++ -g -Wall
LINK = g++ -g
RM = /bin/rm
INCPATH = -I/usr/local/include/opencv
LIBS = -L/usr/local/bin -lhighgui -lstdc++ -lpthread

all: laser motor

clean:
	$(RM) -f *.o *.out laser motor
	cd ./ai && $(RM) -f *.o *.out
	cd ./etc && $(RM) -f *.o *.out
	cd ./device && $(RM) -f *.o *.out
	cd ./internals && $(RM) -f *.o *.out
	cd ./vision && $(RM) -f *.o *.out
	cd ./Projects && $(RM) -f */*.o */*.out

### LASER TEST #####################
laser: Projects/laser/main.o internals/Registry.o vision/Camera.o vision/Window.o
	@echo "== Linking Laser =="
	$(LINK) Projects/laser/main.o internals/Registry.o vision/Camera.o \
	vision/Window.o $(LIBS) -o laser
	@echo "========== Laser compile SUCCESS! =========="
	
Projects/laser/main.o: Projects/laser/main.cpp
	@echo "== Compiling Laser =="
	cd ./Projects/laser && $(COMPILE) $(INCPATH) -c main.cpp

### MOTOR TEST #####################
motor: Projects/motor/main.o device/RCSerializer.o device/Serial.o \
	device/Keyboard.o internals/Thread.o
	@echo "== Linking Motor =="
	$(LINK) Projects/motor/main.o device/RCSerializer.o device/Serial.o \
	device/Keyboard.o internals/Thread.o $(LIBS) -o motor
	@echo "========== Motor compile SUCCESS! =========="
	
Projects/motor/main.o: Projects/motor/main.cpp
	@echo "== Compiling Motor =="
	cd ./Projects/motor && $(COMPILE) $(INCPATH) -c main.cpp


### DEVICE LIBS ####################
device/Keyboard.o: device/Keyboard.cpp device/Keyboard.hpp
	cd ./device && $(COMPILE) $(INCPATH) -c Keyboard.cpp
device/RCSerializer.o: device/RCSerializer.cpp device/RCSerializer.hpp
	cd ./device && $(COMPILE) $(INCPATH) -c RCSerializer.cpp
device/Serial.o: device/Serial.cpp device/Serial.hpp
	cd ./device && $(COMPILE) $(INCPATH) -c Serial.cpp
	
### INTERNAL LIBS ##################
internals/Registry.o: internals/Registry.cpp internals/Registry.hpp
	cd ./internals && $(COMPILE) $(INCPATH) -c Registry.cpp
internals/Thread.o: internals/Thread.cpp internals/Thread.hpp
	cd ./internals && $(COMPILE) $(INCPATH) -c Thread.cpp
	
### VISION LIBS ####################
vision/Camera.o: vision/Camera.cpp vision/Camera.hpp
	cd ./vision && $(COMPILE) $(INCPATH) -c Camera.cpp
vision/Window.o: vision/Window.cpp vision/Window.hpp
	cd ./vision && $(COMPILE) $(INCPATH) -c Window.cpp




############################# OLDER #########################################

###### OLD ##############
LIBPATH = -L/usr/local/bin
OPTIONS = -lhighgui -lstdc++

vtest: vtest.o ai/Map.o imagelib/colordim.o imagelib/colorreplace.o imagelib/hough.o \
		imagelib/overlay.o imagelib/sepchannel.o gui/Trackbar.o \
		serial/Serializer.o tts/Festival.o
	g++ -g vtest.o ai/Map.o imagelib/colordim.o imagelib/colorreplace.o imagelib/hough.o \
		imagelib/overlay.o imagelib/sepchannel.o gui/Trackbar.o \
		serial/Serializer.o tts/Festival.o $(LIBPATH) $(OPTIONS) -o vtest

followtarget: followtarget.o gui/Trackbar.o imagelib/colordim.o imagelib/colorreplace.o \
		serial/Serializer.o tts/Festival.o imagelib/pixeldensity.o
	g++ -g followtarget.o gui/Trackbar.o imagelib/colordim.o imagelib/colorreplace.o \
		serial/Serializer.o tts/Festival.o imagelib/pixeldensity.o \
		$(LIBPATH) $(OPTIONS) -o followtarget

### COMPILE #######
vtest.o: vtest.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c vtest.cpp

followtarget.o: followtarget.cpp
	g++ -g -Wall $(INCPATH) $(OPTIONS) -c followtarget.cpp

ai/Map.o: ai/Map.cpp ai/Map.hpp
	cd ./ai && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Map.cpp

imagelib/colordim.o: imagelib/colordim.cpp imagelib/colordim.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c colordim.cpp

imagelib/colorreplace.o: imagelib/colorreplace.cpp imagelib/colorreplace.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c colorreplace.cpp

imagelib/hough.o: imagelib/hough.cpp imagelib/hough.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c hough.cpp

imagelib/pixeldensity.o: imagelib/pixeldensity.cpp imagelib/pixeldensity.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c pixeldensity.cpp

imagelib/overlay.o: imagelib/overlay.cpp imagelib/overlay.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c overlay.cpp

imagelib/sepchannel.o: imagelib/sepchannel.cpp imagelib/sepchannel.hpp
	cd ./imagelib && g++ -g -Wall $(INCPATH) $(OPTIONS) -c sepchannel.cpp

serial/Serializer.o: serial/Serializer.cpp serial/Serializer.hpp
	cd ./serial && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Serializer.cpp

gui/Trackbar.o: gui/Trackbar.cpp gui/Trackbar.hpp
	cd ./gui && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Trackbar.cpp

tts/Festival.o: tts/Festival.cpp tts/Festival.hpp
	cd ./tts && g++ -g -Wall $(INCPATH) $(OPTIONS) -c Festival.cpp

