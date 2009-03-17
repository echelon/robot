/**
 * Motor run test
 */
#include "../../controller/KeyboardThread.hpp"
#include "../../controller/XboxThread.hpp"
#include "../../device/RCSerializer.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	Device::RCSerializer* serial = new Device::RCSerializer();
	serial->Open();

	Controller::KeyboardThread keyboardThread(serial);
	Controller::XboxThread xboxThread(serial);
	
	keyboardThread.start();
	xboxThread.start();

	keyboardThread.join();
	xboxThread.join();

	return 0;
}
