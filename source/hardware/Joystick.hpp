#ifndef Robot_Hardware_Joystick
#define Robot_Hardware_Joystick

#include "Device.hpp"

// TODO: Are all of these headers necessary?
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>
#include <string>

#include <linux/joystick.h>

namespace Hardware {

/**
 * Query a system joystick (can be an Xbox 360 controller, PS3 controller, etc.)
 *
 * Adapted from: 
 * http://coding.derkeiler.com/Archive/General/comp.programming/2007-05/msg00480.html
 */

// TODO: Subclass for specific button mappings for Xbox/PS3 controller?

class Joystick : public Device
{
	public:
		/**
		 * CTOR.
		 * Creates connection and queries for initial parameters.
		 * Opens /dev/input/js{num}
		 */
		Joystick(int num = 0);

		/**
		 * DTOR.
		 * Closes connection. 
		 */
		virtual ~Joystick();


		/**
		 * Open the connection. 
		 */
		void open();

		void updateStatus();
		bool statusHasChanged();
		void printStatus();

		/**
		 * Get button values
		 * TODO: This is specific for the Xbox controller. 
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
		 * TODO: What is this? Used when reading.
		 */
		js_event ev;

		/**
		 * Hardware parameters.
		 */
		int numAxis;
		int numButtons;
		int version;
		//char deviceName[80];
		std::string jsName;

		/**
		 * Axis and button state.
		 */
		std::vector<int> axisPos;
		std::vector<int> buttonPos;

		bool changed;
};
}

#endif
