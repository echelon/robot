#ifndef Robot_Controller_RobotThread
#define Robot_Controller_RobotThread

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../hardware/RCSerializer.hpp"

namespace Controller {
class RobotThread: public Internals::Thread
{
	public:
		RobotThread(Hardware::RCSerializer* ser, Internals::RobotState* rs);
		~RobotThread();

	protected:
		/**
		 * Thread auto-functions
		 */
		void setup();
		void destroy();
		void execute(void*);


	private:
		Hardware::RCSerializer* serial;
		Internals::RobotState* state;
		
};
}

#endif
