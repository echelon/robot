#ifndef Robot_Controller_GameControllerThread
#define Robot_Controller_GameControllerThread

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../device/Joystick.hpp"

/**
 * Control via Xbox 360 controller.
 */
namespace Controller {
class GameControllerThread: public Internals::Thread
{
	public:
		/**
		 * Constructor
		 */
		GameControllerThread( Internals::RobotState* rs);

		/**
		 * Destructor
		 */
		~GameControllerThread();

		/**
		 * Maximum value of any joystick axis.
		 * Should be 32767.
		 */
		const static int MAX_AXIS;

		/**
		 * Maximum value of any joystick axis.
		 * Should be -32767.
		 */
		const static int MIN_AXIS;


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
		 * Provides joystick access.
		 */
		Device::Joystick* joystick;

		/**
		 * Robot state.
		 */
		Internals::RobotState* state;
};
}
#endif
