#include "Festival.hpp"

Festival::Festival()
{
	if ((this->iopipe = popen("festival --pipe", "w")) == NULL) {
		printf("Failure to launch festival.");
	}

	this->timeCalled = 0;
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

	// Make sure we had enough time to say what we last did
	/*int pauseTime = int(ceil(sizeof(this->lastSaid)*0.5)) + 2;

	printf("Time: %i", int(ceil(sizeof(this->lastSaid)*0.9)));

	int pauseTime = 2;
	if(this->lastSaid) {
		//printf("Last said true");
		pauseTime = 2;
		int oPauseTime = int(sizeof(this->lastSaid)*0.1);

		if(oPauseTime > pauseTime) {
			pauseTime = oPauseTime;
		}

	}*/

	int pauseTime = 6;
	if(diff < pauseTime) {
		return;
	}

	this->timeCalled = curtime;
	this->lastSaid = *text;

	char buff[500];

	sprintf(buff, "(SayText \" %s \")", text);
	fputs(buff, this->iopipe);
	fflush(this->iopipe);
}
