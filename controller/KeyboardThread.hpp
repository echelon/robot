#ifndef CONTROLLER_KEYBOARDTHREAD_H
#define CONTROLLER_KEYBOARDTHREAD_H

#include "../internals/Thread.hpp"
#include "../device/Keyboard.hpp"
#include "../device/RCSerializer.hpp"

// TODO: Move to namespace Controller

namespace Controller {

/**
 * Communicate the Serializer via the keyboard.
 */
class KeyboardThread: public Internals::Thread
{
	public:
		KeyboardThread(Device::RCSerializer* ser);
		~KeyboardThread();

		void execute(void*);

	private:
		Device::RCSerializer* serial;

};
}
#endif
