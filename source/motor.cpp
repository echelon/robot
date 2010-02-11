/**
 * Motor run test
 */
#include "controller/KeyboardThread.hpp"
#include "controller/GameControllerThread.hpp"
#include "controller/RobotThread.hpp"
#include "device/RCSerializer.hpp"
#include "internals/MainThreadControl.hpp"
#include "internals/RobotState.hpp"

#include <iostream>
#include <stdio.h>

int main(int argc, char** argv)
{
	Device::RCSerializer* serial = new Device::RCSerializer();
	Internals::RobotState* state = new Internals::RobotState();
	serial->open();

	Controller::KeyboardThread keyboardThread(state);
	Controller::GameControllerThread gameThread(state);

	Controller::RobotThread robotThread(serial, state);
	
	keyboardThread.start();
	gameThread.start();
	robotThread.start();

	Internals::MainThreadControl::wait();

	keyboardThread.stop();
	keyboardThread.join();

	gameThread.stop();
	gameThread.join();

	return 0;
}
