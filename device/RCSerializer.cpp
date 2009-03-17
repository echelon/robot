#include "RCSerializer.hpp"
#include <string.h> // using strlen
#include <errno.h> // errno

namespace Device {

RCSerializer::RCSerializer()
{
	fd = 0;
}

RCSerializer::~RCSerializer()
{
	Close();
}

char* RCSerializer::fw()
{
	flush();
	Write("fw\r");

	char* ret = Read();

	flush();
	return ret;
}

char* RCSerializer::battery()
{
	flush();
	Write("sensor 5\r");

	char* ret = Read();

	flush();
	return ret;
}

void RCSerializer::mogo(int m1, int m2)
{
	char buff[50];
	sprintf(buff, "mogo 1:%d 2:%d\r", m1, m2); // TODO was "mogo 1:1%d 2:1%d\r" Why??
	Write((const char*)buff);
	//writeRead(buff, 1000);
}

void RCSerializer::stop()
{
	Write("stop\r");
}

} // end namespace
