#ifndef __defined_imagelib_dim_hpp
#define __defined_imagelib_dim_hpp

#include <cv.h>
#include <highgui.h>
#include "canvas.hpp"

/**
 * dimColor
 * Dim specified color by a constant percentage if it occurs within a 
 * given threshold of deviation.
 */
IplImage* dimColor(IplImage* src, int rgb[3], int threshold = 20, float percent = 0.5f);
void dimColor(IplImage* src, IplImage* dst, int rgb[3], int threshold = 20, float percent = 0.5f);

/**
 * dimOtherColors
 * Dim colors other than the specified one by a constant percentage if the
 * color occurs within a given threshold of deviation from the target color.
 */
IplImage* dimOtherColors(IplImage* src, int rgb[3], int threshold = 20, float percent = 0.5f);
void dimOtherColors(IplImage* src, IplImage* dst, int rgb[3], int threshold = 20, float percent = 0.5f);

/** 
 * dimGrays
 * Dim out the homogeneous gray values (R=G=B) that occur in the image
 * by a constant percentage. This eccentuates (R!=G!=B)/color pixels.
 */
IplImage* dimGrays(IplImage* src, int threshold = 20, float percent = 0.5f);
void dimGrays(IplImage* src, IplImage* dst, int threshold = 20, float percent = 0.5f);

#endif
