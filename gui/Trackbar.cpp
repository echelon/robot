
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

#include "Trackbar.h"

Trackbar::Trackbar(char windowName, int maxVal, int defVal)
{
	this->value = defVal;

	static int n = 0;
	n++;

	cvCreateTrackbar((const char*)n, (const char*)windowName, &this->value, maxVal, NULL);


}
