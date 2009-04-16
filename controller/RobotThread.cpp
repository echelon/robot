#include "RobotThread.hpp"
#include "../internals/MainThreadControl.hpp"

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
		if(!serial->isOpen()) {
			printf("Serial not open... ending program\n");
			Internals::MainThreadControl::signal();
			return;
		}
	}

	Internals::robot_state_t lastState;
	bool motorSent = false;
	bool blinkSent = false;

	while(!stopFlag) {
		Internals::robot_state_t curState = state->getState();

		if(lastState.m1 != curState.m1 || lastState.m2 != curState.m2) {
			serial->mogo(curState.m1, curState.m2);
		}
		/*if(lastState.l1 != curState.l1 || lastState.l2 != curState.l2) {
			serial->blink(curState.l1, curState.l2);
		}*/

		lastState.m1 = curState.m1;
		lastState.m2 = curState.m2;
		lastState.l1 = curState.l1;
		lastState.l2 = curState.l2;
	}
}


} // end namespace
