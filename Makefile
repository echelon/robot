### VARIABLES #############
C = g++ -g -Wall
LN = g++ -g
RM = /bin/rm -f
CD = cd
INC = -I/usr/local/include/opencv -I/usr/local/include/libfeat \
		`pkg-config --cflags-only-I gtk+-2.0`
LIBS = -L/usr/local/bin -lhighgui -lstdc++ -lpthread -lfeat \
		`pkg-config --libs gtk+-2.0`

all: 
	@echo "Cannot compile all projects in development branch."
	@echo "make [motor, ...]"

.PHONY: clean
clean:
	$(RM) laser motor stereo sift test
	cd ./build && $(RM) */*.o */*.so */*/*.o */*/*.so 

.PHONY: stats
stats:
	@wc -l -w -c -L */*.*pp */*/*.*pp 
	@echo " Lines\tWords\tBytes\tLine\t"


### MOTOR CODE ####################

# XXX: Note the link step and 'libs' rule. If more compile targets are added,
# these will likely both need to change. 
motor: source/motor.cpp libs
	@echo "[compile] motor application"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/motor.cpp
	@$(LN) $(LIBS) build/out/motor.o build/out/*/*.o -o motor
	@chmod +x motor

libs: build/out/controller/*.o build/out/device/*.o build/out/internals/*.o
	@$(CD) .

### CONTROLLER LIBS ###################
build/out/controller/*.o: source/controller/*.hpp source/controller/*.cpp
	@echo "[compile] Controller code"
	@$(CD) ./build/out/controller && $(C) $(INC) -c ../../../source/controller/*.cpp


### DEVICE LIBS ###################
build/out/device/*.o: source/device/*.hpp source/device/*.cpp
	@echo "[compile] Device code"
	@$(CD) ./build/out/device && $(C) $(INC) -c ../../../source/device/*.cpp


### INTERNAL LIBS ###################
build/out/internals/*.o: source/internals/*.hpp source/internals/*.cpp
	@echo "[compile] Internal code"
	@$(CD) ./build/out/internals && $(C) $(INC) -c ../../../source/internals/*.cpp
	
