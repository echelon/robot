#ifndef VISION_CANVAS_H
#define VISION_CANVAS_H

#include <cv.h>
#include <highgui.h>

/** 
 * Following code adapted from "Introduction to programming with OpenCV"
 * http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html
 */

namespace Vision {
/**
 * Image Canvas
 * Simple access to image pixels/channels.
 */
template<class T> class Image
{
	private:
		IplImage* imgp;
	public:
		Image(IplImage* img=0) {imgp=img;}
		~Image(){imgp=0;}
		void operator=(IplImage* img) {imgp=img;}
		inline T* operator[](const int rowIndx) {
		return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
	unsigned char b,g,r;
} RgbPixel;

typedef struct{
	float b,g,r;
} RgbPixelFloat;

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;

} // end namespace
#endif
