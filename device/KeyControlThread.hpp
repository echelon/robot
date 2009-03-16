#ifndef DEVICE_KEYCONTROLTHREAD_H
#define DEVICE_KEYCONTROLTHREAD_H

#include "../internals/Thread.hpp"
#include "Keyboard.hpp"
#include "Serializer.hpp"

namespace Device {
class KeyControlThread: public Internals::Thread
{
/*
Event Handlers

Handling stop/start/stop bursts -
	KT KeyboardThread
	ST SerializerThread

	KT Keypress W
	KT Send "forward" message
	ST Read command
	ST Send "mogo" command
	KT No keypress
	KT Set "halt" message? NO.

	KeyControl class, needs - 
		Intercept command
		Send msg to serializer
		record LastIssuedCommand 
		record TimeLastIssued
		if(TimeLastIssued expire & cmd
		is null) { send "halt" command}

	Serializer
		Intercept command
		Issue command down the line
		Test if command worked.
*/

}

#endif;
