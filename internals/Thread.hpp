#ifndef INTERNALS_THREAD_H
#define INTERNALS_THREAD_H

#include <pthread.h>
#include <sched.h>

namespace Internals {
/**
 * Subclassable thread model
 * Adapted from various internet sources:
 * (1) http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-18.html
 * (2) http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
 * 
 * TODO: Argument passing as in (1)
 */
class Thread
{
	public:
		/**
		 * Thread default constructor initializes flags. Do not forget to call
		 * this from the initialization list of the constructors of any 
		 * subclasses.
		 */
		Thread();

		/**
		 * Destructor - needs to be virtual.
		 */
		virtual ~Thread();

		/**
		 * Start the thread.
		 */
		void start();

		/**
		 * Stop the thread.
		 */
		void stop();

		/**
		 * Wait for the thread to finish.
		 * WARNING: Do not call from the thread itself!
		 */
		void join();

   protected:
		/**
		 * Calls setup() and sets flag.
		 */
		void doSetup();

		/**
		 * Calls destroy() and sets flag.
		 * Useful to call in subclassed destructors to ensure unfinished threads
		 * that are being culled the from outside are fully destroyed.
		 */
		void doDestroy();

		/**
		 * Any setup (constructor-like) for the subclassed thread
		 * that needs to occur.
		 */
		virtual void setup();

		/**
		 * Any destruction (destructor-like) for the subclassed thread
		 * that needs to occur.
		 */
		virtual void destroy();

		/**
		 * Execution code for the subclassed thread.
		 */
		virtual void execute(void*);

		/**
		 * Thread execution.
		 */
		void run();

		/**
		 * Thread entry code.
		 */
		static void* entryPoint(void*);

		/**
		 * Whether setup() has been run.
		 */
		bool wasSetup;

		/**
		 * Whether destroy() has been run.
		 */
		bool wasDestroyed;

		/**
		 * Called by stop() to hault the main loop.
		 */
		bool stopFlag;

   private:
		/**
		 * pthreads thread
		 */
		pthread_t thread;
};
}
#endif
