#include "Thread.hpp"
#include <stdio.h>

namespace Internals {

Thread::Thread(): 
	wasSetup(false), 
	wasDestroyed(false) 
{
	// nothing
}

Thread::~Thread() 
{
	// nothing
}

void Thread::start()
{
	pthread_create(&thread, NULL, &Thread::entryPoint, this);
}

void Thread::join()
{
	pthread_join(thread, NULL); 
}

void* Thread::entryPoint(void* self)
{
	Thread* tp = (Thread*)self;
	tp->run();

	return 0;
}

void Thread::run()
{
	doSetup();
	execute(0);
	doDestroy();
}

void Thread::doSetup()
{
	wasSetup = true;
	setup();
}

void Thread::doDestroy()
{
	if(wasSetup && !wasDestroyed) {
		wasDestroyed = true;
		destroy();
	}
}

void Thread::setup()
{
	// Override - thread setup here
}

void Thread::destroy()
{
	// Override - thread destruction here
}

void Thread::execute(void*)
{
	// Override - thread execution code here
}

} // end namespace
