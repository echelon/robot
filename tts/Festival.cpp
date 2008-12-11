#include "Festival.hpp"

Festival::Festival()
{
	if ((this->iopipe = popen("festival --pipe", "w")) == NULL) {
		printf("Failure to launch festival.");
	}

	//this->timeCalled = time(NULL) - 50;
}

Festival::~Festival()
{
	pclose(this->iopipe);
}


void Festival::say(const char* text)
{
	// Don't fill the buffer with endless things to say.
	int curtime = (int)time(NULL);
	int lasttime = (int)this->timeCalled;
	int diff = curtime - lasttime;

	if(diff < 2) {
		return;
	}

	this->timeCalled = curtime;

	char buff[500];

	sprintf(buff, "(SayText \" %s \")", text);
	fputs(buff, this->iopipe);
	fflush(this->iopipe);
}
