#ifndef CONTROLLER_ROBOTTHREAD_H
#define CONTROLLER_ROBOTTHREAD_H

#include "../internals/Thread.hpp"
#include "../internals/RobotState.hpp"
#include "../device/RCSerializer.hpp"

namespace Controller {
class RobotThread: public Internals::Thread
{
	public:
		RobotThread(Device::RCSerializer* ser, Internals::RobotState* rs);
		~RobotThread();

	protected:
		/**
		 * Thread auto-functions
		 */
		void setup();
		void destroy();
		void execute(void*);


	private:
		Device::RCSerializer* serial;
		Internals::RobotState* state;
		
};
}

#endif
