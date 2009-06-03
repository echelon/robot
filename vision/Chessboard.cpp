#include "Chessboard.hpp"
#include <stdio.h>

namespace Vision {

// TODO: Rename member vars
// This code based on O'Reilly's text on OpenCV

Chessboard::Chessboard(CvSize size, int numStore) :
	boardSize(size),
	gray(0),
	numToStore(numStore),
	numStored(0)
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

Chessboard::~Chessboard()
{
	delete[] corners;

	if(gray) {
		cvReleaseImage(&gray);
	}
}

int Chessboard::findCorners(IplImage* img, bool getSubpix)
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
	}

	return found;
}

int Chessboard::findAndDrawCorners(IplImage* img, bool getSubpix)
{
	found = findCorners(img, getSubpix);
	cvDrawChessboardCorners(img, boardSize, corners, cornerCnt, found);
	return found;
}

void Chessboard::drawCorners(IplImage* img)
{
	cvDrawChessboardCorners(img, boardSize, corners, cornerCnt, found);
}

void Chessboard::storeCorners()
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

bool Chessboard::foundEnough()
{
	return (numStored == numToStore);
}

void Chessboard::calibrateCamPrep(IplImage* img)
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
}

IplImage* Chessboard::calibrateFrame(IplImage* img)
{
	IplImage* uimg = cvCloneImage(img);
	cvRemap(uimg, img, mapx, mapy);

	return uimg;
}

} // end namespace
