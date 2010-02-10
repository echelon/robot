#include "Serial.hpp"

#include <fcntl.h> // file control
#include <sys/select.h> // select()
#include <stdio.h>
#include <string.h>
#include <unistd.h>	// UNIX standard functions
#include <stdexcept>

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
	// Flush input, output, or both
	if(queue != TCIFLUSH && queue != TCOFLUSH && queue != TCIOFLUSH) {
		queue = TCIOFLUSH;
	}
	tcflush(fd, queue);
}


int Serial::select(int microseconds, int seconds, bool chkRead, bool chkWrite, bool chkError)
{
	struct timeval tv;
	fd_set rfds; // Check if line is available to read
	fd_set wfds; // Check if line is available to write
	fd_set efds; // Check if line has an error condition pending

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

char* Serial::read(unsigned int bytes)
{
	if(!isOpen()) {
		printf("Serial, Can't read from a non-open file.\n");
		return 0;
	}

	pthread_mutex_lock(&mutex);
	char* ret = doRead(bytes);
	flush(); // TODO (Still not working?)
	pthread_mutex_unlock(&mutex);

	return ret;
}

bool Serial::write(const char* data, bool priority)
{
	if(!isOpen()) {
		printf("Serial, Can't write to a non-open file.\n");
		return false;
	}

	pthread_mutex_lock(&mutex);
	bool ret = doWrite(data, priority);
	flush(); // TODO (Still not working?)
	pthread_mutex_unlock(&mutex);

	return ret;
}

char* Serial::writeRead(const char* inBuff, unsigned int readBytes)
{
	bool  wrRet;
	char* rdRet;

	pthread_mutex_lock(&mutex);
	wrRet = doWrite(inBuff);

	if(!wrRet) {
		pthread_mutex_unlock(&mutex);
		printf("Serial::writeRead, DID NOT WRITE\n"); // keep
		return 0;
	}

	rdRet = (char*)doRead(readBytes);
	flush(); // TODO (Still not working?)

	pthread_mutex_unlock(&mutex);
	
	return rdRet;
}


char* Serial::doRead(unsigned int bytes)
{
	if(bytes < 1) {
		return 0;
	}

	std::string buff;
	int failcnt = 0;

	while(buff.length() < bytes) 
	{
		int r = select(50500, 0, true, false, false);
		if(r < 0) {
			printf("Serial::doRead, Error with select()\n");
			return 0; // TODO EXCEPTION
		}
		else if(r == 0) {
			printf("Serial::doRead, LINE NOT AVAILABLE!!!!\n");
			break;
		}

		char rbuf[1000];
		int re = ::read(fd, rbuf, bytes-buff.length());
		if(!re) {
			if(failcnt >= 5) {
				printf("Serial::read, Fail count over 5.\n");
				return 0; // DON'T RETURN PARTIAL DATA :(
			}
			failcnt++;
			continue;
		}
		failcnt = 0;

		buff += rbuf;

		// TODO NOTE: PySerial had an additional check here for timeouts.
		// I think my code is fine, but if errors occur this may be a reason.
	}
	//tcflush(fd, TCIOFLUSH);

	return (char*)buff.c_str();
}

bool Serial::doWrite(const char* data, bool priority)
{
	//
	// TODO/NOTE: Timing was removed. If needed, can see old code in diffs. (4/21/09)
	//

	std::string buff(data);
	int len = buff.length();

	while(len > 0) 
	{
		int r = select(60500, 0, false, true, false);
		if(r < 0) {
			printf("Serial::doWrite, Error with select()\n");
			return false; // TODO EXCEPTION
		}
		else if(r == 0) {
			printf("Serial::doWrite, LINE NOT AVAILABLE!!!!!\n");
			return false;
		}

		int written = ::write(fd, buff.c_str(), len);
		buff.erase(0, written);
		len -= written;
	}
	clock_gettime(CLOCK_REALTIME, &lastWrite);
	return true;
}



} // end namespace