#include "RCSerializer.hpp"
#include <string.h>
#include <stdio.h>

namespace Device {

RCSerializer::RCSerializer() :
	serial(0),
	isOwnsSerial(true)
{
	serial = new Serial();
}

RCSerializer::RCSerializer(Serial* ser, bool grantOwner) :
	serial(0),
	isOwnsSerial(grantOwner)
{
	serial = serial;
}

RCSerializer::~RCSerializer()
{
	serial->close();

	if(isOwnsSerial) {
		delete serial;
	}
}

char* RCSerializer::fw()
{
	serial->flush();
	serial->write("fw\r");

	char* ret = serial->read();

	serial->flush();
	return ret;
}

char* RCSerializer::battery()
{
	serial->flush();
	serial->write("sensor 5\r");

	char* ret = serial->read();

	serial->flush();
	return ret;
}

bool RCSerializer::mogo(int m1, int m2)
{
	if(m1 == 0 && m2 == 0) {
		// send prioritized stop message instead
		return stop(); 
	}

	char buff[50];
	sprintf(buff, "mogo 1:%d 2:%d\r", m1, m2);

	char* read = serial->writeRead((const char*)buff);

	if(!checkAck(read)) {
		return false;
	}
	return true; 
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

bool RCSerializer::blink(int r1, int r2)
{
	char buff[50];

	if(r1 < 0 && r2 < 0) {
		printf("Blink rates need to be set\n");
		return false;
	}

	if(r1 > 127 || r2 > 127) {
		printf("Blink rates out of bounds\n");
		return false;
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

 	return serial->write((const char*)buff);
}

bool RCSerializer::stop()
{
	char* read = serial->writeRead("stop\r");

	if(!checkAck(read)) {
		return false;
	}
	return true; 
}

bool RCSerializer::isOpen()
{
	return serial->isOpen();
}

void RCSerializer::open()
{
	serial->open();
}

bool RCSerializer::checkAck(char* read)
{
	const char* ack = strstr((const char*)read, "ACK\r\n>");
	if(ack != 0) {
		return true;
	}

	const char* nack = strstr((const char*)read, "NACK\r\n>");
	if(nack != 0) {
		printf("RCSerializer::checkAck, request generated a NACK response.\n");
		return false;
	}

	if(read == 0) {
		printf("RCSerializer::checkAck, no data was received in acknowledgement.\n");
		return false;
	}

	printf("RCSerializer::checkAck, response recieved wasn't understood.\n");
	return false;
}


} // end namespace
