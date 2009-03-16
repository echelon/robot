#ifndef DEVICE_SERIAL_H
#define DEVICE_SERIAL_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>		// File control
#include <errno.h>
#include <termios.h>	// POSIX terminal control
#include <unistd.h>		// UNIX standard functions
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h> // for mutex locks, eg as in fw()

#include <iostream> // TODO: Test
#include <fstream>

#include <stropts.h> // ioctl

namespace Device {
/**
 * Serial
 * Performs serial I/O.
 */
class Serial
{
	public:
		
		/**
		 * Default constructor
		 */
		Serial();

		/**
		 * Destructor - calls close
		 */
		~Serial();

//=================== CONNECTION ===================//
// The read, write, and readWrite methods all have
// static mutex locks on them to keep the line safe.
//==================================================//

		/**
		 * Open connection - set up baud rates, parity, etc.
		 * TODO: Do not fix to /dev/ttyUSB0
		 */
		void Open();

		/**
		 * Is the connection handler open?
		 */
		bool isOpen();
		
		/**
		 * Close the connection.
		 */
		void Close();

		/**
		 * Flush a queue with tcflush. Provide the queue to be flushed,
		 * or both input and output queues are flushed by default.
		 */
		void flush(int queue = TCIOFLUSH);

		/**
		 * Wait for the line to open.
		 */
		int Select(int nanoseconds = 50500, int seconds = 0);

		/**
		 * Read a specified number of bytes from the line.
		 */
		char* Read(int bytes = 1000); // TODO: Test if this name conflicts

		/**
		 * Write a specified number of bytes of a character buffer 
		 * to the line.
		 */
		void Write(const char* data); // TODO: Test if this name conflicts

		/**
		 * Write to the line, then read the response.
		 */
		char* writeRead(const char* inBuff, int writeBytes = 1000, int readBytes = 1000);

		char* test(const char* input);

	private:
		/**
		 * File descriptor of an open connection.
		 */
		int fd;

		/**
		 * Termios options for the connection.
		 */	
    	struct termios options;

		/**
		 * Mutex for writing/reading the line
		 */
		static pthread_mutex_t lineMutex;
};
}
#endif
