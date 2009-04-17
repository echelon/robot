#ifndef DEVICE_SERIAL_H
#define DEVICE_SERIAL_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <termios.h>	// POSIX terminal control
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h> // for mutex locks, eg as in fw()

#include <iostream> // TODO: Test
#include <fstream>

#include <stropts.h> // ioctl
#include <time.h>

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

		/**
		 * Open connection - set up baud rates, parity, etc.
		 * TODO: Do not fix to /dev/ttyUSB0
		 */
		void open();

		/**
		 * Is the connection handler open?
		 */
		bool isOpen();
		
		/**
		 * Close the connection.
		 */
		void close();

		/**
		 * Flush a queue with tcflush. Provide the queue to be flushed,
		 * or both input and output queues are flushed by default.
		 */
		void flush(int queue = TCIOFLUSH);

		/**
		 * Read a specified number of bytes from the line.
		 */
		char* read(int bytes = 1000);

		/**
		 * Write a specified number of bytes of a character buffer 
		 * to the line.
		 * @priority messages skip ahead in queues (if any) and don't get rejected
		 * @return bool wasWritten
		 */
		bool write(const char* data, bool priority = false); 

		/**
		 * Write to the line, then read the response.
		 */
		char* writeRead(const char* inBuff, int writeBytes = 1000, int readBytes = 1000);

		char* test(const char* input);

	protected:
		/**
		 * Wait for the line to open.
		 */
		int select(int nanoseconds = 50500, int seconds = 0);

		/**
		 * File descriptor of an open connection.
		 */
		int fd;

		/**
		 * Termios options for the connection.
		 */	
    	struct termios options;

		/**
		 * Time of last write().
		 */
		timespec lastWrite;

		/**
		 * Mutex for writing/reading the line
		 */
		static pthread_mutex_t lineMutex;

};
}
#endif
