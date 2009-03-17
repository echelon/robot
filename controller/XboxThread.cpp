#include "XboxThread.hpp"

namespace Controller {

XboxThread::XboxThread(Device::RCSerializer* ser)
{
	serial = ser;
}

XboxThread::~XboxThread()
{
	// nothing
}

void XboxThread::execute(void*)
{
	if(!serial->isOpen()) {
		serial->Open();
		if(!serial->isOpen()) {
			printf("Serial not open... ending xbox controller thread\n");
			return;
		}
	}

	Device::Joystick joystick;

	if(!joystick.isOpen()) {
		printf("Couldn't establish connection with Xbox controller... abort\n");
		return;
	}

	int max = 32767;
	int min = -32767;

	while(1) {

		joystick.updateStatus();

		if(!joystick.statusHasChanged()) {
			continue;
		}

		if(joystick.getLogoButton()) {
			serial->mogo(0,0);
			serial->stop();
			break;
		}

		if(joystick.getBButton()) {
			serial->mogo(0,0);
			serial->stop();
			continue;
		}

		int l = joystick.getLYAxis();
		int r = joystick.getRYAxis();
		
		printf("L: %d R: %d\n", l, r);

		int speedL = l / (327*7) * -1;
		int speedR = r / (327*7) * -1;

		printf("SL: %d SR: %d\n", speedL, speedR);

		//joystick.printStatus();

		serial->mogo(speedR, speedL);
	}
}

} // end namespace
