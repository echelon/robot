#include "RobotThread.hpp"
#include "../internals/MainThreadControl.hpp"

#include <stdio.h>

namespace Controller {

RobotThread::RobotThread(Device::RCSerializer* ser, Internals::RobotState* rs)
{
	serial = ser;
	state = rs;
}

RobotThread::~RobotThread()
{
	// nothing
}


void RobotThread::setup()
{
	// nothing
}

void RobotThread::destroy()
{
	Internals::MainThreadControl::signal();
}

void RobotThread::execute(void*)
{
	if(!serial->isOpen()) {
		serial->open();
		// TODO: 2/10/10 - Consider adding a wait condition here.
		if(!serial->isOpen()) {
			printf("Serial not open... ending program\n");
			Internals::MainThreadControl::signal();
			return;
		}
	}

	Internals::robot_state_t curState;

	while(!stopFlag) {
		curState = state->getState();

		// Motor
		if(!curState.isMotorWritten()) {
			printf("RobotThread, attempting to mogo(%d, %d)...\n", 
				curState.m1, curState.m2);
			bool written = serial->mogo(curState.m1, curState.m2);
			if(written) {
				printf("RobotThread, motor state was written!\n");
				state->setStateWritten(curState);
			}
			else {
				printf("RobotThread, MOTOR STATE WAS NOT WRITTEN\n");
			}
			continue;
		}

		// Blink
		if(!curState.isBlinkWritten()) {
			printf("RobotThread, attempting to blink(%d, %d)...\n", 
				curState.l1, curState.l2);
			bool written = serial->blink(curState.l1, curState.l2);
			if(written) {
				printf("RobotThread, blink state was written!\n");
				state->setStateWritten(curState);
			}
			else {
				printf("RobotThread, BLINK STATE WAS NOT WRITTEN\n");
			}
			continue;
		}

	} // end mainloop
}


} // end namespace
