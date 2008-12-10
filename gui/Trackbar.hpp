#ifndef __defined_gui_Trackbar_hpp
#define __defined_gui_Trackbar_hpp

#include <cv.h>
#include <highgui.h>

/**
 * Trackbar
 * Easy to create cvTrackbar with current value.
 */
class Trackbar
{
	public:
		int value;
		Trackbar(const char* windowName, const char* trackName, int maxVal, int defVal);
};

#endif
