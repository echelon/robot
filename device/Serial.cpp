#include "Serial.hpp"
#include "../controller/SerialThread.hpp"
#include <unistd.h>	// UNIX standard functions
#include <fcntl.h> // file control
#include <string.h> // using strlen
#include <errno.h> // errno
#include <sys/select.h> // select
#include <stdexcept>
#include <math.h> // abs

namespace Device {

pthread_mutex_t Serial::lineMutex = PTHREAD_MUTEX_INITIALIZER;

Serial::Serial(bool useThread):
	fd(0),
	isUsingThread(false)
{
	if(useThread) {
		printf("Creating SerialThead instance.\n");
		isUsingThread = true;
		serialThread = new Controller::SerialThread(this);
	}
}

Serial::~Serial()
{
	close();

	if(isUsingThread) {
		delete serialThread;
	}
}

void Serial::open() // named to avoid clashes
{
	if(isOpen()) {
		//throw std::runtime_error("Serial is already open");
		printf("Serial is already open\n");
		return;
	}

	fd = ::open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK); 

	if (fd == -1) {
		throw std::runtime_error("Unable to open /dev/ttyUSB0");
	}

	printf("/dev/ttyUSB open on %d\n", fd);

	// alter the current options
    tcgetattr(fd, &options);

	// 19200 baud
    cfsetispeed(&options, B19200);
    cfsetospeed(&options, B19200);

	// Set serial connection options for Serializer.
	// "The SerializerTM ships communicating at 19200 baud, 8 data bits, 
	//  1 stop bit, No Parity, and no Flow Control."

	// Control flags
	options.c_cflag |= CLOCAL;		// Ignore status lines
	options.c_cflag |= CREAD;		// Enable receiver
	options.c_cflag |= CS8;			// Eight data bits
	options.c_cflag &= ~CSTOPB;		// One stop bit (vs two)
	options.c_cflag &= ~(PARENB|PARODD); // No parity
	options.c_cflag &= ~CRTSCTS;	// No RTS/CTS control flow

	// Input flags
	options.c_iflag &= ~(INLCR|IGNCR|ICRNL|IGNBRK);
	options.c_iflag &= ~(INPCK|ISTRIP);
	options.c_iflag &= ~(IXON|IXOFF|IXANY);	// No software flow control
    options.c_iflag &= ~IUCLC;
    options.c_iflag &= ~PARMRK;

	// Output and local mode flags
	options.c_oflag &= ~(OPOST);
	options.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG|IEXTEN);

	// flush I/O & set options
	tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &options);
}

bool Serial::isOpen() 
{
	return (fd > 0); // 0 is stdin
}

void Serial::close()
{
	if(isOpen()) {
		printf("Closing connection...\n");
		::close(fd);
	}
}

void Serial::flush(int queue)
{
	if(queue != TCIFLUSH && queue != TCOFLUSH && queue != TCIOFLUSH) {
		queue = TCIOFLUSH;
	}
	tcflush(fd, queue);
}


int Serial::select(int nanoseconds, int seconds)
{
	struct timeval tv;
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	tv.tv_sec = seconds;
	tv.tv_usec = nanoseconds;

	int r = ::select(fd+1, &rfds, 0, 0, &tv);

	return 1; // TODO: Recent error with select. WTF is going on?
}


// TODO: Will calling writeRead() and Read()/Write() lead to a race condition with the locks?
// What about flush, etc?
void Serial::write(const char* data, bool priority, bool bypassQueue)
{
	if(!isOpen()) {
		// TODO: throw exception
		printf("Writing to a non-open file.\n");
		return;
	}

	// Queue messages
	if(isUsingThread && !bypassQueue) {
		serialThread->enqueue((char*)data, priority);
		serialThread->start();
		return;
	}

	if(!priority) {
		timespec curTime;
		clock_gettime(CLOCK_REALTIME, &curTime);

		long secDif = curTime.tv_sec - lastWrite.tv_sec;
		if(secDif < 1) {
			printf("Sec difference < 2. Ignoring.\n");
			return;
		}
	}
	printf("commit message\n");

	/*long last = ((lastWrite.tv_sec%10) * 10000) + (int)(lastWrite.tv_nsec/chopDigits);
	long cur = ((curTime.tv_sec%10) * 10000) + (int)(curTime.tv_nsec/chopDigits);

	printf("Last: %d . %d\n", lastWrite.tv_sec, lastWrite.tv_nsec);
	printf("Last: %d\n", last);
	printf("Cur: %d . %d\n", curTime.tv_sec, curTime.tv_nsec);
	printf("Cur: %d\n", cur);

	if(lastWrite.tv_sec == curTime.tv_sec) {
		long dif = curTime.tv_nsec - lastWrite.tv_nsec;
		if(dif < 1000) {
			printf("Line busy. Write blocked.\n");
			return;
		}
	}
	else if(lastWrite.tv_sec == curTime.tv_sec-1) {
		long dif = lastWrite.tv_nsec - curTime.tv_nsec;
		if(dif < 1000) {
			printf("Line busy. Write blocked. (2)\n");
			return;
		}

	}
	printf("GOING AHEAD WITH WRITE...\n");*/


	flush(); // TODO (Not working): First motor command on program run fails.

	std::string buff(data);
	int len = buff.length();

	while(len > 0) { // TODO: Wrap entire contents of while loop with try block

		//int r = Select();
		int r = 1; // TODO: Huge problem with select. It used to work...
		if(r < 0) {
			printf("Error with select()\n");
			return; // TODO: exception
		}
		else if(r == 0) {
			printf("Line not available.\n");
			return; // Didn't become available
		}

		int written = ::write(fd, buff.c_str(), len);
		buff.erase(0, written);
		len -= written;

		/* TODO: if (self._timeout >= 0 or self._interCharTimeout > 0) and not buf:
            break   # early abort on timeout*/
		
		/*TODO: catch Exception {
       		if (errno != EAGAIN) {
				// raise error
			}
		}*/
	}
	clock_gettime(CLOCK_REALTIME, &lastWrite);
}


// TODO: Will calling writeRead() and Read()/Write() lead to a race condition with the locks?
// What about flush, etc?
char* Serial::read(int bytes)
{
	if(!isOpen()) {
		// TODO: throw exception
		return 0;
	}

	if(bytes < 1) {
		return 0;
	}
	printf("Testing\n");

	std::string buff;

	int failcnt = 0;
	while(buff.length() < bytes) {

		int r = select();
		if(r < 0) {
			printf("Error with select()\n");
			return 0; // TODO: exception
		}
		else if(r == 0) {
			printf("Line not available.\n");
			break; // Didn't become available
		}

		char rbuf[1000];
		int re = ::read(fd, rbuf, bytes-buff.length());
		if(!re) {
			if(failcnt >= 5) {
				break;
			}
			failcnt++;
			continue;
		}
		failcnt = 0;

		buff += rbuf;

		// TODO NOTE: PySerial had an additional check here for timeouts.
		// I think my code is fine, but if errors occur this may be a reason.
	}
	tcflush(fd, TCIOFLUSH);

	printf("Read from line:\n==============\n%s\n==============\n\n", buff.c_str());
	return (char*)buff.c_str();
}

// TODO: Will calling writeRead() and Read()/Write() lead to a race condition with the locks?
// What about flush, etc?
char* Serial::writeRead(const char* inBuff, int writeBytes, int readBytes)
{
	return 0;
}

} // end namespace
