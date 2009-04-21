#ifndef DEVICE_SERIAL_H
#define DEVICE_SERIAL_H

#include <termios.h>
#include <pthread.h>


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
		char* writeRead(const char* inBuff, int readBytes = 1000);

		char* test(const char* input);

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
		char* doRead(int bytes = 1000);
		bool doWrite(const char* data, bool priority = false); 

};
}
#endif
