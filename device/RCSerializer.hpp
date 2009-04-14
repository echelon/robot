#ifndef DEVICE_RCSERIALIZER_H
#define DEVICE_RCSERIALIZER_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h> // for mutex locks, eg as in fw()

#include <iostream> // TODO: Test
#include <fstream>

#include "Serial.hpp"

namespace Device {
/**
 * RCSerializer
 * Easy to control interface to Robotics Connection's serializer.
 */
class RCSerializer: public Serial
{
	public:
		
		/**
		 * Default constructor
		 */
		RCSerializer();

		/**
		 * Destructor - calls close
		 */
		~RCSerializer();

		/**
		 * Get the firmware version of the serializer
		 */
		char* fw();

		/**
		 * Get the battery reading
		 */
		char* battery();

		/**
		 * Issue a motor speed command
		 */
		void mogo(int m1, int m2);

		/**
		 * Blink the two LEDs
		 * Set only the LED that is meant to be turned on. 
		 */
		void blink(int r1 = -1, int r2 = -1);

		/**
		 * Stop the motor.
		 */
		void stop();

	private:
};
}
#endif
