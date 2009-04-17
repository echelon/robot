/**
 * Motor run test
 */
#include "../../controller/KeyboardThread.hpp"
#include "../../controller/XboxThread.hpp"
#include "../../controller/RobotThread.hpp"
#include "../../device/RCSerializer.hpp"
#include "../../internals/MainThreadControl.hpp"
#include "../../internals/RobotState.hpp"

#include <iostream>
#include <stdio.h>

int main(int argc, char** argv)
{
	Device::RCSerializer* serial = new Device::RCSerializer();
	Internals::RobotState* state = new Internals::RobotState();
	serial->open();

	Controller::KeyboardThread keyboardThread(state);
	Controller::XboxThread xboxThread(state);

	Controller::RobotThread robotThread(serial, state);
	
	keyboardThread.start();
	xboxThread.start();
	robotThread.start();

	Internals::MainThreadControl::wait();

	keyboardThread.stop();
	keyboardThread.join();

	xboxThread.stop();
	xboxThread.join();

	return 0;
}
