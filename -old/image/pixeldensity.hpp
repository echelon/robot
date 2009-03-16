#ifndef __defined_imagelib_pixeldensity_hpp
#define __defined_imagelib_pixeldensity_hpp

#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "canvas.hpp"

bool densityGt(IplImage* src, int rgb[3], float percent, int threshold = 5);

#endif
