#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <termios.h>

/**
 * ADAPTED FROM http://linux-sxs.org/programming/kbhit.html
 */

namespace Device {

class Keyboard
{
	public:

		 Keyboard();
		~Keyboard();
		int kbhit();
		int getch();

	private:

		struct termios initial_settings, new_settings;
		int peek_character;

};
}

#endif
