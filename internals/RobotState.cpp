#include "RobotState.hpp"
#include <stdio.h>

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
	if(m1 == state.m1 && m2 == state.m2) { // TODO: Need to do this within mutex?
		pthread_mutex_unlock(&mutex);
		printf("RobotState, No change in motor state.\n");
		return;
	}

	state.m1 = m1;
	state.m2 = m2;
	//state.motorWritten = false;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::setBlink(int l1, int l2)
{
	pthread_mutex_lock(&mutex);
 	if(l1 == state.l1 && l2 == state.l2) { // TODO: Need to do this within mutex?
		pthread_mutex_unlock(&mutex);
		printf("RobotState, No change in blink state.\n");
		return;
	}

	state.l1 = l1;
	state.l2 = l2;
	//state.blinkWritten = false;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::stopMotors()
{
	pthread_mutex_lock(&mutex);
 	if(0 == state.m1 && 0 == state.m2) { // TODO: Need to do this within mutex?
		pthread_mutex_unlock(&mutex);
		printf("RobotState, No change in motor state.\n");
		return;
	}

	state.m1 = 0;
	state.m2 = 0;
	//state.motorWritten = false;
	pthread_mutex_unlock(&mutex); 
}

void RobotState::stopBlink()
{
	pthread_mutex_lock(&mutex); 
 	if(0 == state.l1 && 0 == state.l2) { // TODO: Need to do this within mutex?
		pthread_mutex_unlock(&mutex);
		printf("RobotState, No change in blink state.\n");
		return;
	}

	state.l1 = 0;
	state.l2 = 0;
	//state.blinkWritten = false;
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
	ret.writtenM1 = state.writtenM1;
	ret.writtenM2 = state.writtenM2;
	ret.writtenL1 = state.writtenL1;
	ret.writtenL2 = state.writtenL2;

	pthread_mutex_unlock(&mutex); 

	return ret;
}

void RobotState::setStateWritten(robot_state_t st)
{
	pthread_mutex_lock(&mutex);
	//state.stateWritten = st;
	state.writtenM1 = st.m1;
	state.writtenM2 = st.m2;
	pthread_mutex_unlock(&mutex);
}

void RobotState::printState()
{
	printf("==== ROBOT STATE ====\n");
	printf("Motor 1: %d (%d old)\n", state.m1, state.writtenM1);
	printf("Motor 2: %d (%d old)\n", state.m2, state.writtenM2);
	printf("Blink 1: %d (%d old)\n", state.l1, state.writtenL1);
	printf("Blink 2: %d (%d old)\n\n", state.l2, state.writtenL2);
}

} // end namespace
