/**
 * Motor run test
 */
#include "../../device/Keyboard.hpp"
#include "../../device/RCSerializer.hpp"
#include "../../device/Serial.hpp"
#include "../../internals/Thread.hpp"

#include <iostream>

class KeyboardInterceptor: public Internals::Thread
{
	void execute(void*)
	{

	
		Device::RCSerializer serial;
		serial.Open();

		if(serial.isOpen()) {
			printf("Serial is open\n");
		}
		else {
			printf("Serial not open... ending\n");
			return;
		}

		Device::Keyboard keyboard;
		int ch;
		bool quit = false;
		int cmd = 0;
		char* read;
		while(!quit) {
			if(keyboard.kbhit()) {
				ch = keyboard.getch();
				
				switch(ch) {
					case 119: // w - forward						
						serial.mogo(10, 10);
						printf("Forward\n");
						break;

					case 97: // a - left
						printf("Left\n");
						serial.mogo(-10, 10);
						break;

					case 100: // d - right
						//serial.mogo(5,-5);
						printf("Right\n");
						serial.mogo(10, -10);
						break;

					case 115: // s - back
						//serial.mogo(-5,-5);
						printf("Back\n");
						serial.mogo(-10, -10);
						break;

					case 120: // x - stop
						printf("Stop\n");
						serial.stop();
						break;

					case 102: // f - fw
						cmd++;
						printf("Firmware. Command #%d\n", cmd);
						read = serial.fw();
						printf("%s\n", read);
						break;

					case 114: // r- read
						printf("Read\n");
						read = serial.Read();
						printf("Read:%s\n", read);
						break;

					case 112: // p - purge/clear I/O queues
					case 99:  // c
						printf("Flush I/O queues\n");
						serial.flush();
						break;

					case 110: // n
						printf("Nonsense input command and read\n");
						serial.Write("asdfjhk\r");
						read = serial.Read();
						printf("Read:%s\n", read);
						break;

					case 98: // b
						printf("Nonsense input command WITHOUT read\n");
						serial.Write("asdfjhk\r");
						break;

					case 113: // q
					case 27: // escape - quit
					case 96: // tilde - quit
						printf("Quit\n");
						serial.mogo(0, 0);
						serial.stop();
						quit = true;
						break;

					default:
						printf("Unknown key %d\n", ch);
						break;
				}
				ch = 0;
			}
			//serial.Read();
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
	thread1.join();


	return 0;


	//while(1) {
	//serial.mogo(5, 5);
	//}
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
