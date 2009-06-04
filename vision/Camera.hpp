#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H

#include <cv.h>
#include <highgui.h>
#include "Device/Calibration.hpp" // included here for the sake of API

// TODO: If calibration is made threaded, ensure no race conditions occur.

namespace Vision {
class Camera 
{
	public:
		/**
		 * Query a hardware camera.
		 * Specify the camera number to use, zero as default.
		 */
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
		 * Grabs from queryFrame() but adds chessboard
		 * corners to the image (from the last performed search).
		 */
		IplImage* queryFrameWithChessboard();
		
		/**
		 * Return the original width of the frame, or the resized width.
		 */
		int getWidth();

		/**
		 * Return the original height of the frame, or the resized height.
		 */
		int getHeight();

		/**
		 * Get the calibration object.
		 * If none exists, it will be created here.
		 */
		Device::Calibration* getCalibration();

		/**
		 * Save the configuration.
		 */
		bool saveConfig(const char* filename);

		/**
		 * Load a configuration file.
		 */
		bool loadConfig(const char* filename);

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

		/**
		 * Allocation/deallocation buffer for calibrated images.
		 */
		IplImage* calibratedBuff;

		/**
		 * Calibration tool for this device.
		 */
		Device::Calibration* calibration;

		/**
		 * Initialize the calibration object if not yet created.
		 */
		void initCalibration();
};
}

#endif
