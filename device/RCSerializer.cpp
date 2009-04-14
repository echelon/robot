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
	close();
}

char* RCSerializer::fw()
{
	flush();
	write("fw\r");

	char* ret = read();

	flush();
	return ret;
}

char* RCSerializer::battery()
{
	flush();
	write("sensor 5\r");

	char* ret = read();

	flush();
	return ret;
}

void RCSerializer::mogo(int m1, int m2)
{
	char buff[50];

	printf("mogo(%d, %d)\n", m1, m2);

	sprintf(buff, "mogo 1:%d 2:%d\r", m1, m2); // TODO was "mogo 1:1%d 2:1%d\r" Why??
	write((const char*)buff);
	//writeRead(buff, 1000);
}


void RCSerializer::blink(int r1, int r2)
{
	char buff[50];

	printf("blink(%d, %d)\n", r1, r2);

	if(r1 < 0 && r2 < 0) {
		printf("Blink rates need to be set\n");
		return;
	}

	if(r1 > 127 || r2 > 127) {
		printf("Blink rates out of bounds\n");
		return;
	}

	if(r1 >= 0 && r2 < 0) {
		sprintf(buff, "blink 1:%d\r", r1);
	}
	else if(r1 < 0 && r2 >= 0) {
		sprintf(buff, "blink 2:%d\r", r2);
	}
	else {
		sprintf(buff, "blink 1:%d 2:%d\r", r1, r2);
	}

 	write((const char*)buff);

}

void RCSerializer::stop()
{
	write("stop\r");
}

} // end namespace
