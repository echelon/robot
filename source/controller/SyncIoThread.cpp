#include "SyncIoThread.hpp"
#include "../internals/MainThreadControl.hpp"

namespace Controller {

SyncIoThread::SyncIoThread(Internals::RobotState* rs)
{
	state = rs;
}

SyncIoThread::~SyncIoThread()
{
	doDestroy();
}

void SyncIoThread::setup()
{
	//joystick = new Hardware::Joystick;
	// Have an (A)syncIo object?

}

void SyncIoThread::destroy()
{
	//delete joystick;
	Internals::MainThreadControl::signal();
}

void SyncIoThread::execute(void*)
{
	/*if(!joystick->isOpen()) {
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

		int lspeed = (int)(lyp*fullSpeed);
		int rspeed = (int)(ryp*fullSpeed);


		if(lspeed > fullSpeed/2 && rspeed == 0) {
			rspeed = lspeed/2;
		}
		else if(rspeed > fullSpeed/2 && lspeed == 0) {
			lspeed = rspeed/2;
		}

		printf("SyncIoThread, LS: %d RS: %d\n", lspeed, rspeed);

		//joystick->printStatus();

		state->setMotors(rspeed, lspeed);
	}*/
}

} // end namespace Controller
