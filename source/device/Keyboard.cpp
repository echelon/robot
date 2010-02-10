#include "Keyboard.hpp"
#include <unistd.h>
#include <stdio.h>

namespace Device {

Keyboard::Keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    peek_character=-1;

	printf("Grabbing keyboard.\n");
}

Keyboard::~Keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
	printf("Keyboard returned.\n");
}

int Keyboard::kbhit()
{
	unsigned char ch;
	int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if (nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

int Keyboard::getch()
{
	char ch;

    if (peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
    } else read(0,&ch,1);

    return ch;
}

} // end namespace
