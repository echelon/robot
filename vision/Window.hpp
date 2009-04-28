#ifndef VISION_WINDOW_H
#define VISION_WINDOW_H

#include <cv.h>
#include <highgui.h>
#include <vector>

namespace Vision 
{

/**
 * This class encapsulates OpenCV's High GUI.
 */
class Window 
{
	public:
		/**
		 * Default constructor.
		 * Does nothing. 
		 */
		Window() {}

		/**
		 * Constructor.
		 */
		Window(const char* name);

		/**
		 * Destructor.  
		 */
		~Window();

		/**
		 * Show an image in the window.
		 */
		void showImage(IplImage* image);

		/**
		 * Show an image in the window, and draw a histogram above it.
		 */
		void showImageWithHist(IplImage* image);

		/**
		 * Create a named trackbar.
		 */
		void createTrackbar(const char* name, int maxVal, int defVal = 0, 
							CvTrackbarCallback callback = 0);

		/**
		 * Get a named trackbar's value.
		 */
		int getTrackbarValue(const char* name);


		/**
		 * OpenCV checks if a key was pressed. Use this to break
		 * the main loop.
		 */
		static bool keypress();

	private:
		/**
		 * Name of the window.
		 */
		const char* windowName;

		/**
		 * For each allocated trackbar, we also allocate an int that it refers
		 * to by reference. 
		 */
		std::vector<int> trackbarValues;
};
}

#endif