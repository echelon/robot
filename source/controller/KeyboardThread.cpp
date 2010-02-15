#include "KeyboardThread.hpp"
#include "../internals/MainThreadControl.hpp"
#include <stdio.h>

namespace Controller {

KeyboardThread::KeyboardThread(Internals::RobotState* rs): Thread()
{
	state = rs;
}

KeyboardThread::~KeyboardThread()
{
	doDestroy();
}

void KeyboardThread::setup()
{
	keyboard = new Hardware::Keyboard();
}

void KeyboardThread::destroy()
{
	delete keyboard;
	Internals::MainThreadControl::signal();
}

void KeyboardThread::execute(void*)
{
	int ch;
	int lastCh;
	//char* read;

	while(!stopFlag) {
		if(keyboard->kbhit()) {
			lastCh = ch;
			ch = keyboard->getch();

			if (lastCh == ch) {
				continue;
			}
			printf("KeyboardThread, new state.\n");
			
			switch(ch) {
				case 49: // 1
					state->setBlink(0,0);
					break;
				case 50: // 2
					state->setBlink(0,50);
					break;
				case 51: // 3
					state->setBlink(50,0);
					break;
				case 52: // 4
					state->setBlink(0,100);
					break;
				case 53: // 5
					state->setBlink(100,0);
					break;
				case 119: // w - forward						
					state->setMotors(100, 100);
					break;
				case 97: // a - left
					state->setMotors(-100, 100);
					break;

				case 100: // d - right
					state->setMotors(100, -100);
					break;

				case 115: // s - back
					state->setMotors(-100, -100);
					break;

				case 120: // x - stop
					printf("KeyboardThread, Stop\n");
					state->stopMotors();
					break;

				case 102: // f - fw // TODO
					//printf("Firmware\n");
					//read = serial->fw();
					//printf("%s\n", read);
					break;

				case 98: // b - battery // TODO
					//printf("Battery\n");
					//read = serial->battery();
					//printf("%s\n", read);
					break;

				case 114: // r- read TODO
					//printf("Read\n");
					//read = serial->read();
					//printf("Read:%s\n", read);
					break;

				case 112: // p - purge/clear I/O queues
				case 99:  // c
					//printf("Flush I/O queues\n");
					//serial->flush();
					break;

				case 110: // n
					//printf("Nonsense input command and read\n");
					//serial->write("asdfjhk\r");
					//read = serial->read();
					//printf("Read:%s\n", read);
					break;

				case 109: // m
					//printf("Nonsense input command WITHOUT read\n");
					//serial->write("asdfjhk\r");
					break;

				case 113: // q
				case 27: // escape - quit
				case 96: // tilde - quit
					printf("KeyboardThread, Quit\n");
					state->stopMotors();
					stop();
					break;

				default:
					printf("Unknown key %d\n", ch);
					break;
			}
			//ch = 0;
		}
	}
}

} // end namespace
