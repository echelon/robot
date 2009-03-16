#include "Thread.hpp"

namespace Internals {

Thread::Thread() {}

void Thread::start()
{
	/*Arg(arg); // store user data
	int code = thread_create(Thread::EntryPoint, this, & ThreadId_);
	return code;*/

	pthread_create(&thread, NULL, &Thread::entryPoint, this); // TODO: Does this work?
}

void Thread::join()
{
	pthread_join(thread, NULL); 
}

void Thread::run()
{
	setup();
	execute(0);
}

/*static */
void* Thread::entryPoint(void* self) // TODO: was void* Thread::entryPoint(void * pthis)
{
	Thread* tp = (Thread*)self;
	tp->run();

	return 0; // TODO: Temp
}

void Thread::setup()
{
	// Do any thread setup here
}

void Thread::execute(void*)
{
	// Thread execution code here
}

} // end namespace
