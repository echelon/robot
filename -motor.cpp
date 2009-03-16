
#include "device/Keyboard.hpp"
#include "device/Serializer.hpp"

#include "internals/Thread.hpp"

#include <iostream>

class KeyboardInterceptor: public Internals::Thread
{
	void execute(void*)
	{
		Device::Keyboard keyboard;
		int ch;
		bool quit = false;
		while(!quit) {
			if(keyboard.kbhit()) {
				ch = keyboard.getch();
				
				switch(ch) {
					case 119: // w - forward
						//serial.mogo(5,5);
						printf("Forward\n");
						break;

					case 97: // a - left
						//serial.mogo(-5,5);
						printf("Left\n");
						break;

					case 100: // d - right
						//serial.mogo(5,-5);
						printf("Right\n");
						break;

					case 115: // s - back
						//serial.mogo(-5,-5);
						printf("Back\n");
						break;

					case 120: // x - stop
						//serial.mogo(0,0);
						printf("Stop\n");
						break;

					case 113: // q - quit
						//serial.mogo(0,0);
						printf("Quit\n");
						quit = true;
						break;
					default:
						printf("Unknown key %d\n", ch);
						break;
				}
			}
		}
	}
};
class ThreadB: public Internals::Thread
{
	void execute(void*)
	{
		while(1) {
			//printf("Execute ThreadB\n");
		}
	}
};

int main(int argc, char** argv)
{
	KeyboardInterceptor thread1;
	ThreadB thread2;
	
	thread1.start();
	thread2.start();

	while(1) {
		//printf("Main\n");
	}
	
	
	/* WORKS
	Device::Keyboard keyboard;
	Device::Serializer serial;
	int ch;
	bool quit = false;
	while(!quit) {
		if(keyboard.kbhit()) {
			ch = keyboard.getch();
			switch(ch) {
				case 119: // w - forward
					serial.mogo(5,5);
					printf("Forward\n");
					break;

				case 97: // a - left
					serial.mogo(-5,5);
					printf("Left\n");
					break;

				case 100: // d - right
					serial.mogo(5,-5);
					printf("Right\n");
					break;

				case 115: // s - back
					serial.mogo(-5,-5);
					printf("Back\n");
					break;

				case 120: // x - stop
					serial.mogo(0,0);
					printf("Stop\n");
					break;

				case 113: // q - quit
					serial.mogo(0,0);
					printf("Quit\n");
					quit = true;
					break;
				default:
					printf("Unknown key %d\n", ch);
					break;
			}
		}
	}
	*/
	return 0;
}
