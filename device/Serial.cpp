#include "Serial.hpp"
#include <unistd.h>	// UNIX standard functions
#include <fcntl.h> // file control
#include <string.h> // using strlen
#include <errno.h> // errno
#include <sys/select.h> // select
#include <stdexcept>
#include <math.h> // abs

namespace Device {

Serial::Serial():
	fd(0),
	mutex((pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER)
{
	// nothing
}

Serial::~Serial()
{
	close();
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


int Serial::select(int microseconds, int seconds, bool chkRead, bool chkWrite, bool chkError)
{
	struct timeval tv;
	fd_set rfds; // Check if available to read
	fd_set wfds; // Check if available to write
	fd_set efds; // Check if an error condition is pending

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	if (chkRead) {
		FD_SET(fd, &rfds);
	}
	if(chkWrite) {
		FD_SET(fd, &wfds);
	}
	if(chkError) {
		FD_SET(fd, &efds);
	}

	tv.tv_sec = seconds;
	tv.tv_usec = microseconds;

	int r = ::select(fd+1, &rfds, &wfds, &efds, &tv);

	return r;
}

bool Serial::write(const char* data, bool priority)
{
	printf("Serial::write, Attempt to write:\n\t%s\n", data);
	if(!isOpen()) {
		printf("Serial, Can't write to a non-open file.\n");
		return false;
	}

	//
	// TIMING
	//

	/*if(!priority) {
		timespec curTime;
		clock_gettime(CLOCK_REALTIME, &curTime);

		long secDif = curTime.tv_sec - lastWrite.tv_sec;
		if(secDif == 0) {
			printf("Serial, Sec difference==0. Ignoring.\n");
			return false;
		}
	}
	printf("Serial, ATTEMPT WRITE\n");*/

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

	//
	// WRITE
	//

	flush(); // TODO (Not working): First motor command on program run fails.

	std::string buff(data);
	int len = buff.length();

	pthread_mutex_lock(&mutex);
	while(len > 0) 
	{
		int r = select(60500, 0, false, true, false);
		if(r < 0) {
			printf("Serial, Error with select()\n");
			return false;
		}
		else if(r == 0) {
			printf("Serial, LINE NOT AVAILABLE!!!!!\n");
			return false;
		}

		int written = ::write(fd, buff.c_str(), len);
		buff.erase(0, written);
		len -= written;
	}
	pthread_mutex_unlock(&mutex);
	clock_gettime(CLOCK_REALTIME, &lastWrite);

	printf("Serial, WRITE \"COMMITTED\"! Data: \n\t%s\n", data);
	return true;
}


char* Serial::read(int bytes)
{
	if(!isOpen()) {
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

char* Serial::writeRead(const char* inBuff, int writeBytes, int readBytes)
{
	return 0;
}

} // end namespace
