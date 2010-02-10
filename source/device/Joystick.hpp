#ifndef DEVICE_JOYSTICK_H
#define DEVICE_JOYSTICK_H

// TODO: Are all of these headers necessary?
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>

#include <linux/joystick.h>

namespace Device {

/**
 * Query the Xbox 360 controller (as a system joystick)
 *
 * Adapted from: 
 * http://coding.derkeiler.com/Archive/General/comp.programming/2007-05/msg00480.html
 */

// TODO: this should be a generic joystick subclassed by an Xbox joystick
// TODO: Xbox controller interface should work under xpad and xboxdrv as well
// 		 as their respective mappings

class Joystick 
{
	public:
		/**
		 * Instantiate axcess interface for the device - creates a 
		 * connection and queries for initial parameters.
		 */
		Joystick();

		/**
		 * Close the open connection.
		 */
		~Joystick();

		bool isOpen();

		void updateStatus();
		bool statusHasChanged();
		void printStatus();

		/**
		 * Get button values
		 */
		int getAButton();
		int getBButton();
		int getXButton();
		int getYButton();
		int getLButton();
		int getRButton();
		int getBackButton();
		int getStartButton();
		int getLogoButton();
		int getLClickButton();
		int getRClickButton();

		/**
		 * Get axis values
		 */
		int getLXAxis();
		int getLYAxis();
		int getRXAxis();
		int getRYAxis();
		int getRTrigAxis();
		int getLTrigAxis();
		int getDpadXAxis();
		int getDpadYAxis();

	private:
		/**
		 * File descriptor.
		 */
		int fd;

		/**
		 * TODO: What is this? Used when reading.
		 */
		js_event ev;

		/**
		 * Hardware parameters.
		 */
		int numAxis;
		int numButtons;
		int version;
		char deviceName[80];

		/**
		 * Axis and button state.
		 */
		std::vector<int> axisPos;
		std::vector<int> buttonPos;

		bool changed;
};
}

#endif
