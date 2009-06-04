#include "HighGuiWindow.hpp"
#include <stdio.h>

namespace Vision {

HighGuiWindow::HighGuiWindow(const char* name) : 
	windowName(name)
{
	cvNamedWindow(windowName, 1);
}

HighGuiWindow::~HighGuiWindow()
{
	cvDestroyWindow(windowName);
}

void HighGuiWindow::showImage(IplImage* image)
{
	cvShowImage(windowName, image);
}

void HighGuiWindow::showImageWithHist(IplImage* image)
{
	IplImage* gray;

	if(image->nChannels == 3) {
		gray = cvCreateImage(cvGetSize(image), 8, 1);
		cvCvtColor(image, gray, CV_RGB2GRAY);
	}
	else if (image->nChannels == 1) {
		gray = cvCloneImage(image);
	}
	else { // TODO: Need to rework
		gray = cvCreateImage(cvGetSize(image), 8, 1); 
		cvCvtColor(image, gray, CV_RGB2GRAY); //TODO: Don't assume this!
	}

	int boxHeight = image->height / 8;

	int bins = 256;
	int hsize[] = {bins};

	float xranges[] = { 0, 256 }; // grayscale 0 - 255
	float* ranges[] = { xranges };
 
	IplImage* planes[] = { gray };

	CvHistogram* hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges,1);
	cvCalcHist(planes, hist, 0, NULL);

	float max = 0;
	float min = 0;
	cvGetMinMaxHistValue(hist, &min, &max);

	// Draw histogram in lower right corner
	for(int i = 0; i < 256; i++) {
		float value = cvQueryHistValue_1D(hist, i);
		int normalized = cvRound(value*boxHeight/max);
		cvLine(
			image, 
			cvPoint((image->width-256)+i, image->height), 
			cvPoint((image->width-256)+i, image->height-normalized), 
			CV_RGB(255, 255, 255)
		);
		/*cvLine(
			frame, 
			cvPoint((getWidth()-256)+i, getHeight()-normalized), 
			cvPoint((getWidth()-256)+i, getHeight()-boxHeight), 
			CV_RGB(0, 0, 0)
		);*/
	}

	cvReleaseImage(&gray);
	cvReleaseHist(&hist);

	cvShowImage(windowName, image);
}

void HighGuiWindow::createTrackbar(const char* name, 
		int maxVal, int defVal, CvTrackbarCallback callback)
{
	trackbarValues.push_back(defVal);
	cvCreateTrackbar(name, windowName, &trackbarValues.back(), maxVal, callback);
}

int HighGuiWindow::getTrackbarValue(const char* name)
{
	return cvGetTrackbarPos(name, windowName);
}

bool HighGuiWindow::keypress()
{
	return ((cvWaitKey(10) & 255) == 27);
}


} // end namespace
