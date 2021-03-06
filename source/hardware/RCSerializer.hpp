#ifndef Robot_Device_RCSerializer
#define Robot_Device_RCSerializer
/**
 * Copyright (c) 2008 - 2010 Brandon Thomas Suit
 * http://possibilistic.org
 * echelon@gmail.com
 * Code available under the AGPL version 3. 
 *
 * Description
 * 
 * Hardware::RCSerializer
 * Control interface to Robotics Connection's serializer. Currently, it supports
 * only a USB serial connection, but could be updated to support the bluetooth
 * or other communication modules. 
 */

#include "Serial.hpp"
#include <string>

namespace Hardware {
class RCSerializer
{
	public:
		
		/**
		 * CTOR
		 */
		RCSerializer();

		/**
		 * CTOR - pass serial connection object.
		 * If ownership is granted, DTOR destroys serial connection too. 
		 */
		RCSerializer(Serial* ser, bool grantOwnership = true);

		/**
		 * DTOR
		 */
		~RCSerializer();

		/**
		 * Get the firmware version of the serializer
		 */
		std::string fw();

		/**
		 * Get the battery reading
		 */
		std::string battery();

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

		/**
		 * TODO: Refactor/remove these passthru methods
		 * DEPRECATE
		 */
		bool isOpen();
		void open();

	protected:
		/**
		 * Serial connection for the RCSerializer.
		 * This is the only method of communcation to serializer I'll be adding.
		 */
		Serial* serial;

		/**
		 * If this object owns the serial object.
		 */
		bool isOwnsSerial;

		/**
		 * Helper function determines if responses were acknowledged.
		 * Pass the data read from the line, and string will be searched for
		 * ACK/NACK sequence. 
		 */
		bool checkAck(std::string read);
};
}
#endif
