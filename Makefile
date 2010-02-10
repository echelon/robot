### VARIABLES #############
COMPILE = g++ -g -Wall
LINK = g++ -g
RM = /bin/rm
INCPATH = -I/usr/local/include/opencv -I/usr/local/include/libfeat \
		`pkg-config --cflags-only-I gtk+-2.0`
LIBS = -L/usr/local/bin -lhighgui -lstdc++ -lpthread -lfeat \
		`pkg-config --libs gtk+-2.0`

all: 
	@echo "Cannot compile all projects in development branch."
	@echo "make [motor, ...]"

.PHONY: clean
clean:
	$(RM) -f *.o *.a *.out laser motor stereo sift test
	cd ./controller && $(RM) -f *.o *.out
	cd ./device && $(RM) -f *.o *.out
	cd ./internals && $(RM) -f *.o *.out

### MOTOR TEST #####################
motor: motor.o \
	device/RCSerializer.o device/Serial.o device/Joystick.o device/Keyboard.o \
	internals/Thread.o internals/MainThreadControl.o internals/RobotState.o \
	controller/KeyboardThread.o controller/XboxThread.o controller/RobotThread.o
	@echo "== Linking Motor =="
	$(LINK) motor.o \
	device/RCSerializer.o device/Serial.o device/Joystick.o device/Keyboard.o \
	internals/Thread.o internals/MainThreadControl.o internals/RobotState.o \
	controller/KeyboardThread.o controller/XboxThread.o controller/RobotThread.o \
	$(LIBS) -o motor
	@echo "========== Motor compile SUCCESS! =========="
	@clear
	@echo "Motor project compiled successfully."
	
motor.o: motor.cpp
	@echo "== Compiling Motor =="
	$(COMPILE) $(INCPATH) -c motor.cpp

### CONTROLLER LIBS ################
controller/KeyboardThread.o: controller/KeyboardThread.cpp controller/KeyboardThread.hpp
	cd ./controller && $(COMPILE) $(INCPATH) -c KeyboardThread.cpp
controller/XboxThread.o: controller/XboxThread.cpp controller/XboxThread.hpp
	cd ./controller && $(COMPILE) $(INCPATH) -c XboxThread.cpp
controller/RobotThread.o: controller/RobotThread.cpp controller/RobotThread.hpp
	cd ./controller && $(COMPILE) $(INCPATH) -c RobotThread.cpp

### DEVICE LIBS ####################
device/Joystick.o: device/Joystick.cpp device/Joystick.hpp
	cd ./device && $(COMPILE) $(INCPATH) -c Joystick.cpp
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
internals/MainThreadControl.o: internals/MainThreadControl.cpp internals/MainThreadControl.hpp
	cd ./internals && $(COMPILE) $(INCPATH) -c MainThreadControl.cpp
internals/RobotState.o: internals/RobotState.cpp internals/RobotState.hpp
	cd ./internals && $(COMPILE) $(INCPATH) -c RobotState.cpp
	
### VISION LIBS ####################
vision/Camera.o: vision/Camera.cpp vision/Camera.hpp
	cd ./vision && $(COMPILE) $(INCPATH) -c Camera.cpp
vision/HighGuiWindow.o: vision/HighGuiWindow.cpp vision/HighGuiWindow.hpp
	cd ./vision && $(COMPILE) $(INCPATH) -c HighGuiWindow.cpp
vision/GtkWindowThread.o: vision/GtkWindowThread.cpp vision/GtkWindowThread.hpp
	cd ./vision && $(COMPILE) $(INCPATH) -c GtkWindowThread.cpp
vision/Device/Calibration.o: vision/Device/Calibration.cpp vision/Device/Calibration.hpp
	cd ./vision/Device && $(COMPILE) $(INCPATH) -c Calibration.cpp
vision/Device/CalibrationThread.o: vision/Device/CalibrationThread.cpp vision/Device/CalibrationThread.hpp
	cd ./vision/Device && $(COMPILE) $(INCPATH) -c CalibrationThread.cpp

