#ifndef Robot_Hardware_Joystick
#define Robot_Hardware_Joystick
/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available under the AGPL version 3.
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

#include "Device.hpp"

#include <linux/joystick.h>
#include <vector>
#include <string>

namespace Hardware {
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
		std::string jsName;

		/**
		 * Axis and button state.
		 */
		std::vector<int> axisPos;
		std::vector<int> buttonPos;

		/**
		 * If the status has changed.
		 */
		bool changed;
};
}

#endif
