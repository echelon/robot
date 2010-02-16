#ifndef Robot_Hardware_Keyboard
#define Robot_Hardware_Keyboard
/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available under the GPL version 3.
 *
 * Description
 * 
 * Hardware::Keyboard
 * A class used to query the keyboard.
 * Originally adapted from: http://linux-sxs.org/programming/kbhit.html
 */

#include <termios.h>

namespace Hardware {
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
