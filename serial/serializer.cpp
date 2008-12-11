#include "serializer.hpp"

Serializer::Serializer()
{
	this->fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);

    struct termios options;
    tcgetattr(this->fd, &options); // current options

	// 19200 baud, 8 data bits, 1 stop bit, No Parity, and no Flow Control.

	// 8 data bits
    options.c_cflag &= ~CSIZE; // Mask the character size bits 
    options.c_cflag |= CS8;    // Select 8 data bits 

	// 19200 baud
    cfsetispeed(&options, B19200);
    cfsetospeed(&options, B19200);

	// No parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

    options.c_cflag |= (CLOCAL | CREAD); // enable receiver, set local mode

	// Disable flow control
	options.c_cflag &= ~CRTSCTS;


    tcsetattr(this->fd, TCSANOW, &options); // set options



	printf("File %i\n", this->fd);
	printf("test\n");

	if (this->fd == -1) {
		perror("Unable to open /dev/ttyUSB0");
	}
}

Serializer::~Serializer()
{
	close(this->fd);
}

void Serializer::wr(char* buff)
{
	// Write 1000 bytes. Chances are we won't exceed that. 
	write(this->fd, buff, 1000);
}

char* Serializer::rd(int bytes)
{
	char* buff = 0;

	// Read 1000 bytes by default. Chances are we won't exceed it. 
	read(this->fd, buff, bytes);
	return buff;
}

void Serializer::mogo(int m1, int m2)
{
	char buff[50];
	sprintf(buff, "mogo 1:1%d 2:1%d\r", m1, m2);
	wr(buff);
}

bool Serializer::isOpen() 
{
	return (this->fd != -1);
}



