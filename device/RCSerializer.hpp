#ifndef DEVICE_RCSERIALIZER_H
#define DEVICE_RCSERIALIZER_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h> // for mutex locks, eg as in fw()

#include <iostream> // TODO: Test
#include <fstream>

#include "Serial.hpp"

namespace Device {
/**
 * RCSerializer
 * Easy to control interface to Robotics Connection's serializer.
 */
class RCSerializer: public Serial
{
	public:
		
		/**
		 * Default constructor
		 */
		RCSerializer();

		/**
		 * Destructor - calls close
		 */
		~RCSerializer();

//=================== CONNECTION ===================//
// The read, write, and readWrite methods all have
// static mutex locks on them to keep the line safe.
//==================================================//

		/**
		 * Open connection - set up baud rates, parity, etc.
		 * TODO: Do not fix to /dev/ttyUSB0
		 */
		//void Open();

		/**
		 * Is the connection handler open?
		 */
		//bool isOpen();
		
		/**
		 * Close the connection.
		 */
		//void Close();

		/**
		 * Read a specified number of bytes from the line.
		 */
		//char* Read(); // TODO: Test if this name conflicts

		/**
		 * Write a specified number of bytes of a character buffer 
		 * to the line.
		 */
		//void Write(char* data); // TODO: Test if this name conflicts

		/**
		 * Write to the line, then read the response.
		 */
		//char* writeRead(char* inBuff, int writeBytes = 1000, int readBytes = 1000);

//=================== Robot Instruction Set ===================//
// In this section, we abstract the hardware instruction set.
// Names are kept consistant with the instruction set so that
// they become familiar in both OO and messaging perspectives. 
//============================================================//

		/**
		 * Get the firmware version of the serializer
		 */
		char* fw();

		/**
		 * Issue a motor speed command
		 */
		void mogo(int m1, int m2);

		/**
		 * Stop the motor.
		 */
		void stop();

	private:
};
}
#endif
