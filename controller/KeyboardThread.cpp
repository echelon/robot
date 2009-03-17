#include "KeyboardThread.hpp"

namespace Controller {

KeyboardThread::KeyboardThread(Device::RCSerializer* ser)
{
	serial = ser;
}

KeyboardThread::~KeyboardThread()
{
	// nothing
}

void KeyboardThread::execute(void*)
{
	if(!serial->isOpen()) {
		serial->Open();
		if(!serial->isOpen()) {
			printf("Serial not open... ending keyboard thread\n");
			return;
		}
	}

	Device::Keyboard keyboard;
	int ch;
	bool quit = false;
	char* read;

	while(!quit) {
		if(keyboard.kbhit()) {
			ch = keyboard.getch();
			
			switch(ch) {
				case 119: // w - forward						
					serial->mogo(10, 10);
					printf("Forward\n");
					break;

				case 97: // a - left
					printf("Left\n");
					serial->mogo(-10, 10);
					break;

				case 100: // d - right
					//serial->mogo(5,-5);
					printf("Right\n");
					serial->mogo(10, -10);
					break;

				case 115: // s - back
					//serial->mogo(-5,-5);
					printf("Back\n");
					serial->mogo(-10, -10);
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
					read = serial->Read();
					printf("Read:%s\n", read);
					break;

				case 112: // p - purge/clear I/O queues
				case 99:  // c
					printf("Flush I/O queues\n");
					serial->flush();
					break;

				case 110: // n
					printf("Nonsense input command and read\n");
					serial->Write("asdfjhk\r");
					read = serial->Read();
					printf("Read:%s\n", read);
					break;

				case 109: // m
					printf("Nonsense input command WITHOUT read\n");
					serial->Write("asdfjhk\r");
					break;

				case 113: // q
				case 27: // escape - quit
				case 96: // tilde - quit
					printf("Quit\n");
					serial->mogo(0, 0);
					serial->stop();
					quit = true;
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
