/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available under the GPL version 3.
 *
 * Description
 * 
 * Hardware::Joystick (a Hardware::Device)
 * Query a system joystick (can be an Xbox 360 controller, PS3 controller, etc.)
 *
 * Linux-specific.
 * 
 * TODO: Subclass for specific button mappings for Xbox/PS3 controller.
 *		 Perhaps create a JoystickFactory to accomplish correct mappings.
 */

#include "Joystick.hpp"

#include <fcntl.h> // open(), etc.
#include <sstream>
#include <stdexcept>
#include <stdio.h>

namespace Hardware {

Joystick::Joystick(int num):
	Device(),
	numAxis(0),
	numButtons(0),
	version(0),
	axisPos(0),
	buttonPos(0)
{
	std::ostringstream os;

	os << "/dev/input/js" << num;
	devName = os.str();

	open();
}

Joystick::~Joystick()
{
	// Nothing specific. 
	// ~Device() should close
}

void Joystick::open()
{
	char jsNameBuf[80];

	if(isOpen()) {
		//throw std::runtime_error("Joystick is already open");
		fprintf(stderr, "Joystick is already open\n");
		return;
	}

	fd = ::open(devName.c_str(), O_RDONLY);
	if(fd < 1) {
		throw std::runtime_error("Unable to open /dev/input/js#.");
		return;
	}

	printf("Joystick open on %d\n", (int)fd);

	// Get hardware info
	ioctl(fd, JSIOCGAXES, &numAxis);
	ioctl(fd, JSIOCGBUTTONS, &numButtons);
	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGNAME(80), &jsNameBuf); // XXX: Is this ok?
	fcntl(fd, F_SETFL, O_NONBLOCK);

	jsName = jsNameBuf;

	axisPos.clear();
	buttonPos.clear();
	axisPos.resize(numAxis, 0);
	buttonPos.resize(numButtons, 0);
}

void Joystick::updateStatus()
{
	// Zero everything
	changed = false;
	//for (int i = 0; i < numAxis; i++) axisPos[i] = 0;
	//for (int i = 0; i < numButtons; i++) buttonPos[i] = 0;

	// TODO: Got rid of the main loop. Need a way to ensure data is read 
	// in case of context switch, device not ready, etc. select()?

	int r = read(fd, &ev, sizeof(ev));
	if(r < 1) {
		// No read occured - perhaps IO is not ready
		return;
	}

	switch (ev.type) {
		case JS_EVENT_INIT:
		case JS_EVENT_INIT | JS_EVENT_AXIS:
		case JS_EVENT_INIT | JS_EVENT_BUTTON:
			break;
		case JS_EVENT_AXIS:
			axisPos[ev.number] = ev.value;
			changed = true;
			break;
		case JS_EVENT_BUTTON:
			buttonPos[ev.number] = ev.value;
			changed = true;
			break;
		default:
			printf ("Other event ? %d\nnumber=%d\nvalue=%d\n",
				ev.type, ev.number, ev.value);
			break;
	} 
}

bool Joystick::statusHasChanged()
{
	return changed;
}

void Joystick::printStatus()
{
	printf ("----------------------------------------------\n");
	printf ("axis : %d\n", numAxis);
	printf ("buttons : %d\n", numButtons);
	printf ("version : %d\n", version);
	printf ("name : %s\n", jsName.c_str());
	printf ("----------------------------------------------\n");
	printf ("last ev time : %d\n", ev.time);

	for(int i = 0; i < numAxis; i++) printf(" Axis %2d |",i);
	printf("\n");
	for(int i = 0; i < numAxis; i++) printf(" %7d |", axisPos[i]);
	printf("\n\n");

	for(int i = 0; i < numButtons; i++) printf(" Btn.%2d |",i);
	printf("\n");
	for(int i = 0; i < numButtons; i++) printf(" %2d |",buttonPos[i]);
	printf("\n");
	
	changed = false;
}

int Joystick::getAButton() { return buttonPos[0]; }
int Joystick::getBButton() { return buttonPos[1]; }
int Joystick::getXButton() { return buttonPos[2]; }
int Joystick::getYButton() { return buttonPos[3]; }
int Joystick::getLButton() { return buttonPos[4]; }
int Joystick::getRButton() { return buttonPos[5]; }
int Joystick::getBackButton() { return buttonPos[6]; }
int Joystick::getStartButton() { return buttonPos[7]; }
int Joystick::getLogoButton() { return buttonPos[8]; }
int Joystick::getLClickButton() { return buttonPos[9]; }
int Joystick::getRClickButton() { return buttonPos[10]; }

int Joystick::getLXAxis() { return axisPos[0]; }
int Joystick::getLYAxis() { return axisPos[1]; }
int Joystick::getRXAxis() { return axisPos[2]; }
int Joystick::getRYAxis() { return axisPos[3]; }
int Joystick::getRTrigAxis() { return axisPos[4]; } // reversed
int Joystick::getLTrigAxis() { return axisPos[5]; } // reversed
int Joystick::getDpadXAxis() { return axisPos[6]; }
int Joystick::getDpadYAxis() { return axisPos[7]; }

} // end namespace Hardware
