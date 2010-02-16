/**
 * Motor run test
 */
#include "controller/KeyboardThread.hpp"
#include "controller/GameControllerThread.hpp"
#include "controller/RobotThread.hpp"
#include "controller/SyncIoThread.hpp"
#include "hardware/RCSerializer.hpp"
#include "internals/MainThreadControl.hpp"
#include "internals/RobotState.hpp"

#include <iostream>
#include <stdio.h>
#include <stdexcept>

int main(int argc, char** argv)
{
	Hardware::RCSerializer* serial = 0;
	Internals::RobotState* state = 0;

	// Ensure we can plug up the robot if we haven't already
	while(serial == 0) {
		try {
			serial = new Hardware::RCSerializer();
			printf("USB Serial connected.\n");
			break;
		}
		catch(std::runtime_error e) {
			printf("Cannot connect to serial... connect USB cable.\n");
			serial = 0;
			sleep(5);
		}
	}

	state = new Internals::RobotState();

	Controller::KeyboardThread keyboardThread(state);
	Controller::GameControllerThread gameThread(state);
	Controller::SyncIoThread syncioThread(state);

	Controller::RobotThread robotThread(serial, state);
	
	keyboardThread.start();
	gameThread.start();
	syncioThread.start();
	robotThread.start();

	Internals::MainThreadControl::wait();

	keyboardThread.stop();
	keyboardThread.join();

	gameThread.stop();
	gameThread.join();

	syncioThread.stop();
	syncioThread.join();

	return 0;
}
