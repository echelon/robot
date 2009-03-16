#ifndef __defined_tts_Festival_hpp
#define __defined_tts_Festival_hpp

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>


/**
 * Festival TTS
 * TTS Engine is used to output debug and satus info since the
 * user will not be able to see the terminal. 
 */
class Festival
{
	public:
		Festival();
		~Festival();

		void say(const char* text);

	private:
	    FILE *iopipe;
		int timeCalled;
		char lastSaid;
		
};

#endif
