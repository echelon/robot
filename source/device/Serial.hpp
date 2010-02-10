#ifndef Robot_Device_Serial
#define Robot_Device_Serial
/**
 * Copyright (c) 2008 - 2010 Brandon Thomas Suit
 * http://possibilistic.org
 * echelon@gmail.com
 * Code available under the GPL version 3. 
 *
 * Description
 * 
 * Serial (Linux-specific)
 * A mutex-locked access object for USB serial I/O devices. Uses termio and 
 * pthread mutexes. 
 *
 * TODO: better opening/closing semantics. Don't throw exceptions! I want to 
 * be able to start the robot regardless...
 */

#include <termios.h>
#include <pthread.h>
#include <string>

namespace Device {
class Serial
{
	public:
		
		/**
		 * CTOR
		 */
		Serial();

		/**
		 * DTOR
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
		std::string read(unsigned int bytes = 1000);

		/**
		 * Write a specified number of bytes of a character buffer 
		 * to the line.
		 * @priority messages skip ahead in queues (if any) and don't get rejected
		 * @return bool wasWritten
		 */
		bool write(std::string, bool priority = false); 

		/**
		 * Write to the line, then read the response.
		 */
		std::string writeRead(std::string in, unsigned int rbytes = 1000);

		char* test(std::string input);

	protected:
		/**
		 * Wait for the line to open in given time, a non-blocking select.
		 */
		int select(
			int microseconds = 0, // 50500
			int seconds = 0, 
			bool chkRead = false, 
			bool chkWrite = false, 
			bool chkError = false
		);

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
		 * Mutex on read/write
		 */
		pthread_mutex_t mutex;

	private:

		/**
		 * Helper methods perform the actual read/write WITHOUT locking.
		 * By doing this, mutexes can be used in read(), write(), and writeRead().
		 */
		std::string doRead(unsigned int bytes = 1000);
		bool doWrite(std::string data, bool priority = false); 

};
}
#endif
