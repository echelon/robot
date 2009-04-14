#include "MainThreadControl.hpp"

namespace Internals {

pthread_cond_t MainThreadControl::cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t MainThreadControl::mut = PTHREAD_MUTEX_INITIALIZER; 

void MainThreadControl::wait()
{
	pthread_mutex_lock(&mut);
	pthread_cond_wait(&cond, &mut);
	pthread_mutex_unlock(&mut);
}

void MainThreadControl::signal()
{
	pthread_mutex_lock(&mut);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mut);
}

} // end namespace
