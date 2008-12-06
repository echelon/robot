INCPATH = -I/usr/local/include/opencv
LIBPATH = -L/usr/local/bin
OPTIONS = -lhighgui -lstdc++

# top-level rule to create the program.
all: vision

# compiling the source file.
vision.o: vision.cpp
	gcc -g -Wall $(INCPATH) $(OPTIONS) -c vision.cpp

# linking the program.
vision: vision.o
	gcc -g vision.o $(LIBPATH) $(OPTIONS) -o vision

# cleaning everything that can be automatically recreated with "make".
clean:
	/bin/rm -f vision vision.o a.out

