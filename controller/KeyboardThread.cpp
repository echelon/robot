#include "KeyboardThread.hpp"
#include "../internals/MainThreadControl.hpp"

namespace Controller {

KeyboardThread::KeyboardThread(Device::RCSerializer* ser): Thread()
{
	serial = ser;
}

KeyboardThread::~KeyboardThread()
{
	doDestroy();
}

void KeyboardThread::setup()
{
	keyboard = new Device::Keyboard();
}

void KeyboardThread::destroy()
{
	delete keyboard;
	Internals::MainThreadControl::signal();
}

void KeyboardThread::execute(void*)
{
	if(!serial->isOpen()) {
		serial->open();
		if(!serial->isOpen()) {
			printf("Serial not open... ending keyboard thread\n");
			return;
		}
	}

	int ch;
	char* read;

	while(!stopFlag) {
		if(keyboard->kbhit()) {
			ch = keyboard->getch();
			
			switch(ch) {
				case 49: // 1
					serial->blink(0,0);
					printf("Blink 0 0\n");
					break;
				case 50: // 2
					serial->blink(-1,50);
					printf("Blink 0 50\n");
					break;
				case 51: // 3
					serial->blink(50);
					printf("Blink 50 0\n");
					break;
				case 52: // 4
					serial->blink(-1,100);
					printf("Blink 50 50\n");
					break;
				case 53: // 5
					serial->blink(100);
					printf("Blink 50 50\n");
					break;


				case 119: // w - forward						
					serial->mogo(100, 100);
					printf("Forward\n");
					break;

				case 97: // a - left
					printf("Left\n");
					serial->mogo(-100, 100);
					break;

				case 100: // d - right
					//serial->mogo(5,-5);
					printf("Right\n");
					serial->mogo(100, -100);
					break;

				case 115: // s - back
					//serial->mogo(-5,-5);
					printf("Back\n");
					serial->mogo(-100, -100);
					break;

				case 120: // x - stop
					printf("Stop\n");
					serial->stop();
					break;

				case 102: // f - fw
					printf("Firmware\n");
					read = serial->fw();
					printf("%s\n", read);
					break;

				case 98: // b - battery
					printf("Battery\n");
					read = serial->battery();
					printf("%s\n", read);
					break;

				case 114: // r- read
					printf("Read\n");
					read = serial->read();
					printf("Read:%s\n", read);
					break;

				case 112: // p - purge/clear I/O queues
				case 99:  // c
					printf("Flush I/O queues\n");
					serial->flush();
					break;

				case 110: // n
					printf("Nonsense input command and read\n");
					serial->write("asdfjhk\r");
					read = serial->read();
					printf("Read:%s\n", read);
					break;

				case 109: // m
					printf("Nonsense input command WITHOUT read\n");
					serial->write("asdfjhk\r");
					break;

				case 113: // q
				case 27: // escape - quit
				case 96: // tilde - quit
					printf("Quit\n");
					serial->mogo(0, 0);
					serial->stop();
					stop();
					break;

				default:
					printf("Unknown key %d\n", ch);
					break;
			}
			ch = 0;
		}
		//serial->Read();
	}
}

} // end namespace
