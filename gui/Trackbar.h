#include <cv.h>
#include <highgui.h>

#ifndef TRACKBAR_H
#define TRACKBAR_H

class Trackbar
{
	public:
		int value;
		Trackbar(char windowName, int maxVal, int defVal = 0);
};

#endif
