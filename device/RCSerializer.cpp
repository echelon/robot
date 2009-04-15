#include "RCSerializer.hpp"
#include <string.h> // using strlen
#include <errno.h> // errno

namespace Device {

RCSerializer::RCSerializer(bool useThread) : Serial(useThread)
{
	// nothing
}

RCSerializer::~RCSerializer()
{
	close();

	if(isUsingThread) {
		delete serialThread;
	}
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
	printf("mogo disabled\n");
	return;

	if(m1 == 0 && m2 == 0) {
		stop(); // send prioritized stop message instead
		return;
	}

	char buff[50];

	sprintf(buff, "mogo 1:%d 2:%d\r", m1, m2);
	write((const char*)buff);
}

/*void RCSerializer::mogoPercent(double m1, double m2)
{
	char buff[50];

	int fullSpeed = 500;
	int lspeed = (int)(lyp*fullSpeed);
	int rspeed = (int)(ryp*fullSpeed);

	sprintf(buff, "mogo 1:%d 2:%d\r", rspeed, lspeed);
	write((const char*)buff);
}*/

void RCSerializer::blink(int r1, int r2)
{
	char buff[50];

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
	write("stop\r", true);
}

} // end namespace
