/**
 * Motor run test
 */
#include "controller/KeyboardThread.hpp"
#include "controller/GameControllerThread.hpp"
#include "controller/RobotThread.hpp"
#include "controller/SyncIoThread.hpp"
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
	Controller::SyncIoThread syncioThread(state);

	Controller::RobotThread robotThread(serial, state);
	
	keyboardThread.start();
	//gameThread.start();
	syncioThread.start();
	robotThread.start();

	Internals::MainThreadControl::wait();

	keyboardThread.stop();
	keyboardThread.join();

	//gameThread.stop();
	//gameThread.join();

	syncioThread.stop();
	syncioThread.join();

	return 0;
}
