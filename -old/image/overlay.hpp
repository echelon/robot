#ifndef __defined_transform_overlay_hpp
#define __defined_transform_overlay_hpp

#include <cv.h>
#include <highgui.h>
#include "canvas.hpp"

/** 
 * overlayEdges
 * Copies the true pixel values from (any) binary image onto an
 * RBG image. Best used for assessing the results of edge detection
 * or segmentation. 
 */
IplImage* overlayEdges(IplImage* edges, IplImage* other);
 
#endif
