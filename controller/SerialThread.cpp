#include "SerialThread.hpp"
#include "../device/Serial.hpp"
#include <stdio.h>

namespace Controller {

SerialThread::SerialThread(Device::Serial* ser): Thread()
{
	messages = std::queue<SerialMessage>();
	messageMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

SerialThread::~SerialThread()
{
	doDestroy();
}

void SerialThread::setup()
{
	// nothing
}

void SerialThread::destroy()
{
	// nothing (doesn't own RCSerializer)
}

void SerialThread::execute(void*)
{
	printf("Now in execute()\n");
	if(!serial->isOpen()) {
		printf("Serial line is not open!!!!!!!!!!!!!\n");
		serial->open();
		printf("Now it's open\n");
		if(!serial->isOpen()) {
			printf("Serial not open... ending serial thread\n");
			return;
		}
	}

	printf("Execution loop start...\n");

	// Main loop of thread sends queued messages
	while(!stopFlag) {
		// TODO: Do I need to lock while checking size?
		printf("Before mutex acq...\n");
		pthread_mutex_lock(&messageMutex); 
		printf("After mutex acq...\n");
		if(messages.size() < 1) {
			pthread_mutex_unlock(&messageMutex);
			continue;
		}

		SerialMessage message = messages.front();
		messages.pop();
		pthread_mutex_unlock(&messageMutex);

		serial->write(message.text, message.priority, true);
	}
}

void SerialThread::enqueue(char* inst, bool priority)
{
	printf("Message enqueued\n");

	pthread_mutex_lock(&messageMutex);
	if(priority) {
		while(messages.size() > 0) {
			messages.pop();
		}
	}
	SerialMessage message;
	message.text = inst;
	message.priority = priority;

	messages.push(message);
	pthread_mutex_unlock(&messageMutex);
}

} // end namespace
