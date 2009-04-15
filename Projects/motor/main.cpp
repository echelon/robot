/**
 * Motor run test
 */
#include "../../controller/KeyboardThread.hpp"
#include "../../controller/XboxThread.hpp"
#include "../../device/RCSerializer.hpp"
#include "../../internals/MainThreadControl.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	Device::RCSerializer* serial = new Device::RCSerializer(true);
	serial->open();

	Controller::KeyboardThread keyboardThread(serial);
	Controller::XboxThread xboxThread(serial);
	
	keyboardThread.start();
	xboxThread.start();

	Internals::MainThreadControl::wait();

	keyboardThread.stop();
	keyboardThread.join();

	xboxThread.stop();
	xboxThread.join();

	return 0;
}
