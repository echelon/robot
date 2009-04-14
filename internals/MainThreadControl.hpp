#ifndef INTERNALS_MAINTHREADCONTROL_H
#define INTERNALS_MAINTHREADCONTROL_H

#include <pthread.h>
#include <sched.h>

namespace Internals {

/**
 * Main Thread isn't a thread, but a static class that controls signaling 
 * to the main thread of control.
 */
class MainThreadControl
{
	public:
		/**
		 * Cause the main thread to wait on a signal event.
		 * Called by the main thread. 
		 * TODO: DO NOT CALL OUTSIDE MAIN!
		 */
		static void wait();

		/**
		 * Signal the main thread to wake up.
		 * Called by another thread.
		 */
		static void signal();

	private:
		/**
		 * MainThread cannot be instantiated from outside.
		 */
		MainThreadControl() {}
		MainThreadControl(const MainThreadControl&);
		MainThreadControl& operator= (const MainThreadControl&);

		/**
		 * Condition.
		 */
		static pthread_cond_t cond;

		/**
		 * Mutex.
		 */
		static pthread_mutex_t mut;
};
}

#endif
