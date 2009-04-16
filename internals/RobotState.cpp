#include "RobotState.hpp"

namespace Internals {

RobotState::RobotState() :
	state(),
	mutex((pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER)
{
	// NONE
}

RobotState::~RobotState()
{
	// NOTHING YET
}

void RobotState::setMotors(int m1, int m2)
{
	pthread_mutex_lock(&mutex); 
	state.m1 = m1;
	state.m2 = m2;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::setBlink(int l1, int l2)
{
	pthread_mutex_lock(&mutex); 
	state.l1 = l1;
	state.l2 = l2;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::stopMotors()
{
	pthread_mutex_lock(&mutex);
	state.m1 = 0;
	state.m2 = 0;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::stopBlink()
{
	pthread_mutex_lock(&mutex); 
	state.l1 = 0;
	state.l2 = 0;
	pthread_mutex_unlock(&mutex); 
}

/**
 * Returns a copy of the internal state struct.
 */
robot_state_t RobotState::getState()
{
	robot_state_t ret;

	pthread_mutex_lock(&mutex); 

	ret.m1 = state.m1;
	ret.m2 = state.m2;
	ret.l1 = state.l1;
	ret.l2 = state.l2;

	pthread_mutex_unlock(&mutex); 

	return ret;
}


} // end namespace
