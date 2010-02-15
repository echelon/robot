#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <termios.h>

namespace Hardware {

/**
 * Query the Keyboard
 * Adapted from: http://linux-sxs.org/programming/kbhit.html
 */

class Keyboard
{
	public:
		/**
		 * Set up terminal options.
		 */
		Keyboard();

		/**
		 * Restore original terminal options.
		 */
		~Keyboard();

		/**
		 * See if there is a waiting character on the buffer (a keypress event)
		 * TODO: Bool method?
		 */
		int kbhit();

		/**
		 * Get the available keyboard character.
		 */
		int getch();

	private:

		struct termios initial_settings, new_settings;
		int peek_character;

};
}

#endif
