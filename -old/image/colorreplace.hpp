#ifndef __defined_imagelib_colorreplace_hpp
#define __defined_imagelib_colorreplace_hpp

#include <cv.h>
#include <highgui.h>
#include "canvas.hpp"

/**
 * replaceColors
 * Replace colors if they occur within a threshold. 
 */
IplImage* replaceColors(IplImage* src, int colors[][7]);
IplImage* replaceColors(IplImage* src, int colors[][7], int other[3]);
void replaceColors(IplImage* src, IplImage* dst, int colors[][7]);
void replaceColors(IplImage* src, IplImage* dst, int colors[][7], int other[3]);

#endif
