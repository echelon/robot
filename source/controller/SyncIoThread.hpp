#ifndef Robot_Controller_SyncIoThread
#define Robot_Controller_SyncIoThread

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../device/Joystick.hpp"

/**
 * Control via synchronous IO with a client.
 */
namespace Controller {
class SyncIoThread: public Internals::Thread
{
	public:
		/**
		 * Constructor
		 */
		SyncIoThread( Internals::RobotState* rs);

		/**
		 * Destructor
		 */
		~SyncIoThread();

	protected:
		/**
		 * Setup the thread. 
		 * Virtual override, called automatically.
		 */
		void setup();

		/**
		 * "Destroy" the thread.
		 * Virtual override, called automatically.
		 */
		void destroy();

		/**
		 * Instructions for the thread in execution.
		 */
		void execute(void*);

	private:

		/**
		 * Robot state.
		 */
		Internals::RobotState* state;
};
}
#endif
