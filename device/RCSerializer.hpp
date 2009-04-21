#ifndef DEVICE_RCSERIALIZER_H
#define DEVICE_RCSERIALIZER_H

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
		bool mogo(int m1, int m2);

		/**
		 * Blink the two LEDs
		 * Set only the LED that is meant to be turned on. 
		 */
		bool blink(int r1 = -1, int r2 = -1);

		/**
		 * Stop the motor.
		 */
		bool stop();

	protected:
		/**
		 * Helper function determines if responses were acknowledged.
		 */
		bool checkAck(char* read);
};
}
#endif
