/**
 * Copyright (c) 2008 - 2010 Brandon Thomas Suit
 * http://possibilistic.org
 * echelon@gmail.com
 * Code available under the GPL version 3. 
 *
 * Description
 * 
 * Hardware::RCSerializer
 * Control interface to Robotics Connection's serializer. Currently, it supports
 * only a USB serial connection, but could be updated to support the bluetooth
 * or other communication modules. 
 */

#include "RCSerializer.hpp"
#include <stdio.h>
#include <sstream>

namespace Hardware {

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
	std::ostringstream os;

	// send stop message instead of mogo 1:0 2:0
	if(m1 == 0 && m2 == 0) {
		return stop(); 
	}

	os << "mogo 1:" << m1 << " 2:" << m2 << "\r";

	read = serial->writeRead(os.str());

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
	std::ostringstream os;

	if(r1 < 0 && r2 < 0) {
		fprintf(stderr, "Blink rates need to be set\n");
		return false;
	}

	if(r1 > 127 || r2 > 127) {
		fprintf(stderr, "Blink rates out of bounds\n");
		return false;
	}

	if(r1 >= 0 && r2 < 0) {
		os << "blink 1:" << r1 << "\r";
	}
	else if(r1 < 0 && r2 >= 0) {
		os << "blink 2:" << r2 << "\r";
	}
	else {
		os << "blink 1:" << r1 << " 2:" << r2 << "\r";
	}

 	return serial->write(os.str());
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

bool RCSerializer::checkAck(std::string read)
{
	if(read.length() < 1) {
		fprintf(stderr, 
		  "RCSerializer::checkAck, no data was received in acknowledgement.\n");
		return false;
	}

	// Acknowledged 
	if(read.find("ACK\r\n>") != std::string::npos) {
		return true;
	}

	// Not Acknowledged 
	// TODO: May want to output messages on NACK... Consider returning int codes
	// or throwing an exception. 
	if(read.find("NACK\r\n>") != std::string::npos) {
		return false;
	}

	fprintf(stderr, 
		"RCSerializer::checkAck, response recieved wasn't understood.\n");
	return false;
}

} // end namespace Hardware
