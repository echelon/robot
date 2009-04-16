#ifndef CONTROLLER_SERIALTHREAD_H
#define CONTROLLER_SERIALTHREAD_H

#include "../internals/Thread.hpp"
#include <queue>
#include <pthread.h>

// Forward declarations
namespace Device {
	class Serial;
}
//struct pthread_mutex_t;

namespace Controller {

struct SerialMessage {
	char* text;
	bool priority;
};

/**
 * Control the RCSerializer as its own thread. Messages are queued here
 * by other threads and then executed here. 
 */
class SerialThread: public Internals::Thread
{
	public:
		/**
		 * Constructor
		 */
		SerialThread(Device::Serial* ser);

		/**
		 * Destructor
		 */
		~SerialThread();

		/**
		 * Queue messages for execution.
		 */
		void enqueue(char* inst, bool priority = false);

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
		 * The Serial instance to use.
		 */
		Device::Serial* serial;

		/**
		 * Queue of messages.
		 */
		std::queue<SerialMessage> messages;		

		/**
		 * Mutex for message queue.
		 */
		pthread_mutex_t messageMutex;

};
}
#endif
