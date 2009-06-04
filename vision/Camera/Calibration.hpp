#ifndef VISION_CAMERA_CALIBRATION_H
#define VISION_CAMERA_CALIBRATION_H

#include <cv.h>

namespace Vision {
namespace Camera {

/**
 * For camera calibration purposes. Find and store the corners of a chessboard
 * with OpenCV's chessboard calibration algorithms.
 */
class Calibration
{
	public:
		Calibration(CvSize size = cvSize(7,7), int numStore = 5);
		~Calibration();

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

		void calibrateCamPrep(IplImage* img);

		IplImage* calibrateFrame(IplImage* img);

	private:
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
