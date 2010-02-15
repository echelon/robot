#ifndef Robot_Controller_KeyboardThread
#define Robot_Controller_KeyboardThread

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../hardware/Keyboard.hpp"

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
		Hardware::Keyboard* keyboard;

};
}
#endif
