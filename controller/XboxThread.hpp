#ifndef CONTROLLER_XBOXTHREAD_H
#define CONTROLLER_XBOXTHREAD_H

#include "../internals/Thread.hpp"
#include "../device/Joystick.hpp"
#include "../device/RCSerializer.hpp"

namespace Controller {

/**
 * Control via Xbox 360 controller.
 */
class XboxThread: public Internals::Thread
{
	public:
		XboxThread(Device::RCSerializer* ser);
		~XboxThread();

		void execute(void*);

	private:
		Device::RCSerializer* serial;
};
}
#endif
