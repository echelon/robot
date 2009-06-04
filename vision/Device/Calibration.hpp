#ifndef VISION_CAMERA_CALIBRATION_H
#define VISION_CAMERA_CALIBRATION_H

#include <cv.h>

// forward declarations
namespace Vision {
	class Camera;
}

namespace Vision {
namespace Device {

/**
 * For camera calibration purposes. Find and store the corners of a chessboard
 * with OpenCV's chessboard calibration algorithms.
 */
class Calibration
{
	public:
		Calibration(Camera& cam, CvSize size = cvSize(7,7), int numStore = 5);
		~Calibration();

		/**
		 * Determine if calibration has finished (or was performed at all).
		 */
		bool isCalibrated(); // TODO: Don't know about the semantics of this

		/**
		 * Calibrate the camera. Launches as a new thread, so control the
		 * calling code with calls to isCalibrated().
		 * TODO: More advanced thread signalling. 
		 */
		void calibrateThreaded();

		/**
		 * Save calibration data to XML.
		 */
		bool save(const char* filename);

		/**
		 * Load a precalibrated configurations from XML.
		 */
		bool load(const char* filename);



		/**
		 * Find the chessboard corners.
		 */
		int findCorners(IplImage* img, bool getSubpix = true);

		/**
		 * Find and draw the chessboard corners.
		 * Good for debugging. 
		 */
		int findAndDrawCorners(IplImage* img, bool getSubpix = true);

		/**
		 * Draw corners on the chessboard.
		 */
		void drawCorners(IplImage* img);

		void storeCorners();

		bool foundEnough();

		/**
		 * Calculate the needed parameters from the data collected.
		 */
		void finalize(IplImage* img);

		IplImage* calibrateFrame(IplImage* img);

		/**
		 * Access to skip count, the number of frames to skip between captures.
		 */
		unsigned int getSkipCount();
		void setSkipCount(unsigned int s);

		/**
		 * Map accessors.
		 */
		IplImage* getXMap();
		IplImage* getYMap();

		/**
		 * Return camera reference.
		 */
		Camera& getCamera();

	private:
		// TODO NEW VARS
		bool calibrationStarted;
		bool calibrationFinished;
		bool calibrationLoaded;
		unsigned int skipCount;

		// Reference to camera
		Camera& camera;


		// TODO: Rename member vars to be more specific...
		int cornerCnt;
		int found;
		int boardArea;
		CvSize boardSize;
		CvPoint2D32f* corners;

		// Count of frames 
		int numStored;
		int numToStore;

		// For finding subpixel accuracy 
		IplImage* gray;

		// storage
		CvMat* imgPts;
		CvMat* objPts;
		CvMat* pointCounts;

		// storage p2
		CvMat* imgPts2;
		CvMat* objPts2;
		CvMat* pointCounts2;

		// intrensics & distortion
		CvMat* intrensicMat;
		CvMat* distortCoeff;

		// maps
		IplImage* mapx;
		IplImage* mapy;

};
}
}

#endif
