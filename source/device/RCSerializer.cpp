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

std::string RCSerializer::fw()
{
	std::string ret;

	serial->flush();
	serial->write("fw\r");

	ret = serial->read();

	serial->flush();
	return ret;
}

std::string RCSerializer::battery()
{
	std::string ret;

	serial->flush();
	serial->write("sensor 5\r");

	ret = serial->read();

	serial->flush();
	return ret;
}

bool RCSerializer::mogo(int m1, int m2)
{
	std::string read;
	std::string buff;

	if(m1 == 0 && m2 == 0) {
		// send prioritized stop message instead
		return stop(); 
	}

	// TODO (2/10/2010) ostringstream

	//char buff[50];
	//sprintf(buff, "mogo 1:%d 2:%d\r", m1, m2);

	buff = "mogo 1:50 2:50\r"; // XXX: TEMP ONLY

	read = serial->writeRead(buff);

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
	//char buff[50];
	std::string buff; 

	if(r1 < 0 && r2 < 0) {
		printf("Blink rates need to be set\n");
		return false;
	}

	if(r1 > 127 || r2 > 127) {
		printf("Blink rates out of bounds\n");
		return false;
	}

	// XXX TODO: std::ostringstream (2/10/2010)

	/*if(r1 >= 0 && r2 < 0) {
		sprintf(buff, "blink 1:%d\r", r1);
	}
	else if(r1 < 0 && r2 >= 0) {
		sprintf(buff, "blink 2:%d\r", r2);
	}
	else {
		sprintf(buff, "blink 1:%d 2:%d\r", r1, r2);
	}*/

	buff = "blink1:50 2:50\r"; // XXX: TEMP ONLY

 	return serial->write(buff);
}

bool RCSerializer::stop()
{
	std::string read;

	read = serial->writeRead("stop\r");
	return checkAck(read);
}

bool RCSerializer::isOpen()
{
	return serial->isOpen();
}

void RCSerializer::open()
{
	serial->open();
}

// TODO CLEAN UP STRINGS (2/10/2010)
bool RCSerializer::checkAck(std::string read)
{
	const char* ack = strstr((const char*)read.c_str(), "ACK\r\n>");
	if(ack != 0) {
		return true;
	}

	const char* nack = strstr((const char*)read.c_str(), "NACK\r\n>");
	if(nack != 0) {
		printf("RCSerializer::checkAck, request generated a NACK response.\n");
		return false;
	}

	if(read.length() < 1) {
		printf("RCSerializer::checkAck, no data was received in acknowledgement.\n");
		return false;
	}

	printf("RCSerializer::checkAck, response recieved wasn't understood.\n");
	return false;
}


} // end namespace
