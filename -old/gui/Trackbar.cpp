#include "Trackbar.hpp"

/**
 * Trackbar constructor.
 */
Trackbar::Trackbar(const char* windowName, const char* trackName, int maxVal, int defVal)
{
	this->value = defVal;
	cvCreateTrackbar(trackName, windowName, &this->value, maxVal, NULL);
}
