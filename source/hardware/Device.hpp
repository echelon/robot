#ifndef Robot_Hardware_Device
#define Robot_Hardware_Device

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
