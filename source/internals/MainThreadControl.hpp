#ifndef INTERNALS_MAINTHREADCONTROL_H
#define INTERNALS_MAINTHREADCONTROL_H

#include <pthread.h>
#include <sched.h>

namespace Internals {

/**
 * Main Thread isn't a thread, but a static class that controls signaling 
 * to the main thread of control.
 *
 * The main thread can be signaled to end by child threads. The main thread
 * can call wait() or wasSignaled() and handle closure accordingly. 
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
		 * See if the signal to end main was sent.
		 * Called by the main thread.
		 */
		static bool wasSignaled();

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

		/**
		 * Bool (for non-waiting main threads.)
		 */
		static bool tSignal;
};
}

#endif
