#ifndef __defined_transform_hough_hpp
#define __defined_transform_hough_hpp

#include <cv.h>
#include <highgui.h>

/** 
 * houghLines
 * Needs an edge-detected image, such as one from Canny, as the
 * source image. 
 */
void houghLines(IplImage* src, IplImage* dst);
 
#endif
