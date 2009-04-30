#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H

#include <cv.h>
#include <highgui.h>

namespace Vision {
class Camera 
{
	public:
		Camera(int device=0);
		
		~Camera();
		
		/**
		 * Set the camera dimensions.
		 * The cvSetCaptureProperty doesn't seem to work.
		 */
		void setDimensions(int w, int h);
		
		/**
		 * Set frame to be resized.
		 * This will resize the image each time it is requested.
		 */
		void setResize(int w, int h);
		
		/**
		 * Set the FPS
		 */
		void setFps(int fps);

		/**
		 * Grab and retrieve the image from the camera.
		 * Do not release or modify the image that is returned.
		 * Resized if requested by setResize().
		 */
		IplImage* queryFrame();
		
		/**
		 * Grabs from queryFrame() but adds a histogram 
		 * to the image.
		 */
		IplImage* queryFrameWithHist();
		
		/**
		 * Return the original width of the frame, or the resized width.
		 */
		int getWidth();

		/**
		 * Return the original height of the frame, or the resized height.
		 */
		int getHeight();
		
	private:
		/**
		 * Capture structure.
		 */
		CvCapture* capture;
		
		/**
		 * Default resolution.
		 */
		int width;
		int height;
		
		/**
		 * Resize resolutions.
		 */
		int resizeWidth;
		int resizeHeight;

		/**
		 * Automatic allocation/deallocation buffer for queryFrame().
		 */
		IplImage* queryFrameBuff;
	
};
}

#endif
