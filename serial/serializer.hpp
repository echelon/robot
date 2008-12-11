#ifndef __defined_serial_serializer_hpp
#define __defined_serial_serializer_hpp

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>

/**
 * Serializer
 * Easy to control interface to the robot.
 */
class Serializer
{
	public:
		
		Serializer();
		~Serializer();
		bool isOpen();
		
		// Basic read and write to serial line
		char* rd(int bytes = 1000);
		void wr(char* buff);

		// Instruction set
		void mogo(int m1, int m2);

	private:
		int fd; // File descriptor returned by open()
};

#endif
