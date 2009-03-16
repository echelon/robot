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
		 * Thread default constructor does nothing.
		 */
		Thread();

		/**
		 * Start the thread.
		 * TODO: Optionally supply an argument.
		 */
		void start();

		/**
		 * Wait for the thread to finish.
		 * WARNING: Do not call from the thread itself!
		 */
		void join();

   protected:
		/**
		 * Any setup (constructor-like) for the subclassed thread
		 * that needs to occur.
		 */
		virtual void setup();

		/**
		 * Execution code for the subclassed thread.
		 * TODO: Was void execute(void*)
		 */
		virtual void execute(void*);

		/**
		 * Thread execution.
		 * TODO: was int run(void* arg)
		 */
		void run();

		/**
		 * Thread entry code.
		 * TODO: was void* entryPoint(void*)
		 */
		static void* entryPoint(void*);

   private:
		/**
		 * pthreads thread
		 */
		pthread_t thread;
};
}
#endif
