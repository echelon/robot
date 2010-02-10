#ifndef CONTROLLER_KEYBOARDTHREAD_H
#define CONTROLLER_KEYBOARDTHREAD_H

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../device/Keyboard.hpp"

// TODO: Move to namespace Controller

namespace Controller {

/**
 * Communicate the Serializer via the keyboard.
 */
class KeyboardThread: public Internals::Thread
{
	public:
		KeyboardThread(Internals::RobotState* rs);
		~KeyboardThread();

	protected:
		void setup();
		void destroy();
		void execute(void*);

	private:
		Internals::RobotState* state;
		Device::Keyboard* keyboard;

};
}
#endif
