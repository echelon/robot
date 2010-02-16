#ifndef Robot_Hardware_Device
#define Robot_Hardware_Device
/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available under the AGPL version 3.
 *
 * Description
 * 
 * Hardware::Device (Header Only!)
 * An abstraction interface for devices opened with open().
 */

#include <string>
#include <stdio.h>

namespace Hardware {
class Device
{
	public:
		/**
		 * CTOR. 
		 * XXX: Implementor should open the device. 
		 */
		Device() : devName(""), fd(0) {};

		/**
		 * DTOR.
		 * Closes the connection if it is open. 
		 */
		virtual ~Device() {
			if(isOpen()) {
				close();
			}
		};

		/**
		 * Open the device
		 * INTERFACE MUST BE IMPLEMENTED. 
		 */
		virtual void open() = 0;

		/**
		 * Close the device
		 * Can be overridden if required. 
		 */
		virtual void close() {
			if(isOpen()) {
				printf("Closing %s connection...\n", devName.c_str());
				::close(fd);
			}
		};

		/**
		 * If the device is open.
		 */
		bool isOpen() { return (fd > 0); };

	protected:
		/**
		 * Name of the device opened.
		 * eg. /dev/input/js0 or /dev/ttyUSB0
		 */
		std::string devName;

		/**
		 * File descriptor for an open device.
		 */
		int fd;
};
}

#endif
