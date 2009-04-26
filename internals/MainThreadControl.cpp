#include "MainThreadControl.hpp"

namespace Internals {

pthread_cond_t MainThreadControl::cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t MainThreadControl::mut = PTHREAD_MUTEX_INITIALIZER; 
bool MainThreadControl::tSignal = false;

void MainThreadControl::wait()
{
	pthread_mutex_lock(&mut);
	pthread_cond_wait(&cond, &mut);
	pthread_mutex_unlock(&mut);
}

bool MainThreadControl::wasSignaled()
{
	bool sig;
	pthread_mutex_lock(&mut);
	sig = tSignal;
	pthread_mutex_unlock(&mut);
	return sig;
}

void MainThreadControl::signal()
{
	pthread_mutex_lock(&mut);
	pthread_cond_broadcast(&cond);
	tSignal = true;
	pthread_mutex_unlock(&mut);
}

} // end namespace
