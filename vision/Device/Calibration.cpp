#include "Calibration.hpp"
#include "CalibrationThread.hpp"
#include <stdio.h>

namespace Vision {
namespace Device {

// TODO: Rename member vars
// This code based on O'Reilly's text on OpenCV

Calibration::Calibration(Camera& cam, CvSize size, int numStore) :
	boardSize(size),
	gray(0),
	numToStore(numStore),
	numStored(0),
	calibrationStarted(false),
	calibrationFinished(false),
	calibrationLoaded(false),
	skipCount(3), // TODO: TEMP
	camera(cam),
	mapx(0),
	mapy(0)
{
	boardArea = size.width*size.height;
	corners = new CvPoint2D32f[boardArea];

	// Temp Storage Space
	imgPts = cvCreateMat(boardArea*numToStore, 2, CV_32FC1);
	objPts = cvCreateMat(boardArea*numToStore, 3, CV_32FC1);
	pointCounts = cvCreateMat(numToStore, 1, CV_32SC1);

	// Intrensics & distortion
	intrensicMat = cvCreateMat(3, 3, CV_32FC1);
	distortCoeff = cvCreateMat(5, 1, CV_32FC1);
}

Calibration::~Calibration()
{
	delete[] corners;

	if(gray) {
		cvReleaseImage(&gray);
	}
}

int Calibration::findCorners(IplImage* img, bool getSubpix)
{
	found = cvFindChessboardCorners(img, boardSize, corners, &cornerCnt);

	// refine subpixel accuracy
	if(getSubpix) {
		//if(!gray) { // FIXME!!! SEGFAULT
			gray = cvCreateImage(cvGetSize(img), 8, 1);
		//}
		cvCvtColor(img, gray, CV_BGR2GRAY);

		cvFindCornerSubPix(gray, corners, cornerCnt,
			cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(
				CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1
			)
		);
	}

	if(cornerCnt == boardArea) {
		storeCorners();
		printf("Corners found!\n");
	}

	return found;
}

int Calibration::findAndDrawCorners(IplImage* img, bool getSubpix)
{
	found = findCorners(img, getSubpix);
	cvDrawChessboardCorners(img, boardSize, corners, cornerCnt, found);
	return found;
}

void Calibration::drawCorners(IplImage* img)
{
	cvDrawChessboardCorners(img, boardSize, corners, cornerCnt, found);
}

void Calibration::storeCorners()
{
	if(numStored >= numToStore) {
		return;
	}

	// taken from OpenCV book by O'Rielly 
	int step = numStored * boardArea;

	for(int i = step, j = 0; j < boardArea; i++, j++) {
		CV_MAT_ELEM(*imgPts, float,i, 0) = corners[j].x;
		CV_MAT_ELEM(*imgPts, float,i, 1) = corners[j].y;
		CV_MAT_ELEM(*objPts, float,i, 0) = j/boardSize.width;
		CV_MAT_ELEM(*objPts, float,i, 1) = j%boardSize.width;
		CV_MAT_ELEM(*objPts, float,i, 2) = 0.0f;
	}
	CV_MAT_ELEM(*pointCounts, int, numStored, 0) = boardArea;

	numStored++;
}

bool Calibration::foundEnough()
{
	return (numStored == numToStore);
}

void Calibration::finalize(IplImage* img)
{
	printf("\nNumStored: %d\nBoardArea: %d\n\n", numStored, boardArea);

	// Save memory by compacting the storage area
	imgPts2 = cvCreateMat(numStored*boardArea, 2, CV_32FC1);
	objPts2 = cvCreateMat(numStored*boardArea, 3, CV_32FC1);
	pointCounts2 = cvCreateMat(numStored, 1, CV_32SC1);
	
	for(int i = 0; i < numStored*boardArea; i++) {
		CV_MAT_ELEM(*imgPts2, float, i, 0) = 
			CV_MAT_ELEM(*imgPts, float, i, 0);
		CV_MAT_ELEM(*imgPts2, float, i, 1) = 
			CV_MAT_ELEM(*imgPts, float, i, 1);

		CV_MAT_ELEM(*objPts2, float, i, 0) = 
			CV_MAT_ELEM(*objPts, float, i, 0);
		CV_MAT_ELEM(*objPts2, float, i, 1) = 
			CV_MAT_ELEM(*objPts, float, i, 1);
		CV_MAT_ELEM(*objPts2, float, i, 2) = 
			CV_MAT_ELEM(*objPts, float, i, 2);
	}

	// just copy
	for(int i = 0; i < numStored; i++) {
		CV_MAT_ELEM(*pointCounts2, int, i, 0) =
			CV_MAT_ELEM(*pointCounts, int, i, 0);
	}

	cvReleaseMat(&objPts); // TODO: Memleaks
	cvReleaseMat(&imgPts);
	cvReleaseMat(&pointCounts);

	//focal ratio of 1.0
	CV_MAT_ELEM(*intrensicMat, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrensicMat, float, 1, 1) = 1.0f;

	cvCalibrateCamera2(
		objPts2, imgPts2, pointCounts2, 
		cvGetSize(img), intrensicMat, distortCoeff,
		0, 0, 0
	);

	mapx = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
	mapy = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

	cvInitUndistortMap(
		intrensicMat,
		distortCoeff,
		mapx, mapy
	);

	calibrationFinished = true;
}

IplImage* Calibration::calibrateFrame(IplImage* img)
{
	IplImage* uimg = cvCloneImage(img);
	cvRemap(uimg, img, mapx, mapy);

	return uimg;
}

unsigned int Calibration::getSkipCount()
{
	return skipCount;
}

void Calibration::setSkipCount(unsigned int s)
{
	skipCount = s;
}

bool Calibration::load(const char* filename)
{
	return false; // TODO: Implement
}

bool Calibration::save(const char* filename)
{
	return false; // TODO: Implement
}

bool Calibration::isCalibrated()
{
	return (calibrationStarted && calibrationFinished) || calibrationLoaded;
}

Camera& Calibration::getCamera()
{
	return camera;
}


void Calibration::calibrateThreaded()
{
	printf("Testing...");
	if(calibrationStarted) {
		return;
	}
	printf("Testing...2");
	calibrationStarted = true;

	//CalibrationThread cal(*this);
	//cal.start();
}

IplImage* Calibration::getXMap()
{
	return mapx;
}

IplImage* Calibration::getYMap()
{
	return mapy;
}



} // end namespace Camera
} // end namespace Vision