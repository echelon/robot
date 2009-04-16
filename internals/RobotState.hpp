#ifndef INTERNALS_REGISTRY_H
#define INTERNALS_REGISTRY_H

#include <pthread.h>

namespace Internals {
/**
 * Representation of the current robot state.
 */
struct robot_state_t 
{
	int m1;
	int m2;
	int l1;
	int l2;
};

/**
 * Access and mutate the Robot State. One thread will be used to set the state
 * and another thread will push the state down the serial connection.
 */
class RobotState
{
	public:
		RobotState();
		~RobotState();

		void setMotors(int m1, int m2); 
		void setBlink(int l1, int l2);

		void stopMotors();
		void stopBlink();

		/**
		 * Returns a copy of the internal state struct.
		 */
		robot_state_t getState();

	private:
		robot_state_t state;
		pthread_mutex_t mutex;

};
}

#endif
