#include "XboxThread.hpp"
#include "../internals/MainThreadControl.hpp"

namespace Controller {

const int XboxThread::MAX_AXIS = 32767;
const int XboxThread::MIN_AXIS = -32767;

XboxThread::XboxThread(Internals::RobotState* rs)
{
	state = rs;
}

XboxThread::~XboxThread()
{
	doDestroy();
}

void XboxThread::setup()
{
	joystick = new Device::Joystick;
}

void XboxThread::destroy()
{
	delete joystick;
	Internals::MainThreadControl::signal();
}

void XboxThread::execute(void*)
{
	if(!joystick->isOpen()) {
		printf("Couldn't establish connection with Xbox controller... abort\n");
		return;
	}

	while(!stopFlag) {

		joystick->updateStatus();

		if(!joystick->statusHasChanged()) {
			continue;
		}

		if(joystick->getLogoButton()) {
			state->stopMotors();
			state->stopBlink();
			break;
		}

		if(joystick->getBButton()) {
			state->stopMotors();
			state->stopBlink();
			continue;
		}

		int ly = joystick->getLYAxis();
		int ry = joystick->getRYAxis();

		// Convert axis position to percentages and invert the axes
		double lyp = -1* ((double)ly/MAX_AXIS);
		double ryp = -1* ((double)ry/MAX_AXIS);

		
		int fullSpeed = 300;

		// TODO: TEMP - ABSOLUTE VALUE FUNCTION. 
		// Testing with lights instead of motor. (LED uses positive vals only)
		/*if(ly > 0) {
			lyp *= -1;
		}
		if(ry > 0) {
			ryp *= -1;
		}*/

		int lspeed = (int)(lyp*fullSpeed);
		int rspeed = (int)(ryp*fullSpeed);

		printf("XboxThread, LS: %d RS: %d\n", lspeed, rspeed);

		//joystick->printStatus();

		state->setMotors(rspeed, lspeed);
	}
}

} // end namespace
