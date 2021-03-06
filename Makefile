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
	$(RM) laser motor stereo sift test client server
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

libs: build/out/controller/*.o build/out/hardware/*.o build/out/internals/*.o
	@$(CD) .

BINC = -I/usr/include/boost
BLIB = -lboost_system-mt

########################### SERVER CODE (TEST) ###################
server: source/server.cpp
	@echo "[compile] server application"
	$(CD) ./build/out && $(C) $(INC) $(BINC) -c ../../source/server.cpp
	$(LN) $(LIBS) $(BLIB) build/out/server.o -o server
	@chmod +x server

########################### CLIENT CODE (TEST) ###################
client: source/client.cpp
	@echo "[compile] client application"
	$(CD) ./build/out && $(C) $(INC) $(BINC) -c ../../source/client.cpp
	$(LN) $(LIBS) $(BLIB) build/out/client.o -o client
	@chmod +x client


### CONTROLLER LIBS ###################
build/out/controller/*.o: source/controller/*.hpp source/controller/*.cpp
	@echo "[compile] Controller code"
	@$(CD) ./build/out/controller && $(C) $(INC) -c ../../../source/controller/*.cpp


### HARDWARE LIBS ###################
build/out/hardware/*.o: source/hardware/*.hpp source/hardware/*.cpp
	@echo "[compile] Hardware code"
	@$(CD) ./build/out/hardware && $(C) $(INC) -c ../../../source/hardware/*.cpp


### INTERNAL LIBS ###################
build/out/internals/*.o: source/internals/*.hpp source/internals/*.cpp
	@echo "[compile] Internal code"
	@$(CD) ./build/out/internals && $(C) $(INC) -c ../../../source/internals/*.cpp
	
