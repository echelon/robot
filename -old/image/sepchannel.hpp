#ifndef __defined_imagelib_sepchannel_hpp
#define __defined_imagelib_sepchannel_hpp

#include <cv.h>
#include <highgui.h>
#include "canvas.hpp"

/** 
 * sepChannel
 * Extract one channel from an RGB image into grayscale.
 */
IplImage* sepChannel(IplImage* src, char channel);
void sepChannel(IplImage* src, IplImage* dst, char channel);

/** 
 * sepChannel
 * Extract percentages of channels in an RGB image into
 * a resultant RGB. 
 * @usage:
 *   float percents[3] = {0.0f, 0.5f, 1.0f};
 *   IplImage* out = sepChannel(src, percents);
 */
IplImage* sepChannel(IplImage* src, float channels[3]);
void sepChannel(IplImage* src, IplImage* dst, float channels[3]);

#endif
