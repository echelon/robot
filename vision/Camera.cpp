#include "Camera.hpp"
#include "Canvas.hpp"
#include <stdio.h>

namespace Vision {

Camera::Camera(int device)
{
	capture = cvCreateCameraCapture(device);
	if (!capture) {
		// TODO: Throw error
	}

	width  = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	resizeWidth  = 0;
	resizeHeight = 0;
}

Camera::~Camera()
{
	cvReleaseCapture(&capture);
}

void Camera::setDimensions(int w, int h)
{
	// TODO: The following doesn't seem to work. OpenCV build issue?
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, (double)width);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
}

void Camera::setResize(int w, int h)
{
	resizeWidth = w;
	resizeHeight = h;

	if (resizeWidth == width) {
		resizeWidth = 0;
	}
	if (resizeHeight == height) {
		resizeHeight = 0;
	}

	printf("Original Camera - Width: %d, Height: %d\n", width, height);
	printf("Resizing to - Width: %d, Height: %d\n", resizeWidth, resizeHeight);
}

void Camera::setFps(int fps)
{
	cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, (double)fps);
}

IplImage* Camera::queryFrame()
{
	IplImage* frame = cvQueryFrame(capture);

	// Resize if setResize()
	if(resizeWidth && resizeHeight) {
		IplImage* resized = 
			cvCreateImage(cvSize(resizeWidth, resizeHeight), IPL_DEPTH_8U, 3);
		cvResize(frame, resized);

		//cvReleaseImage(&frame); // TODO: Deallocation error
		return resized;
	}
	return frame;
}

IplImage* Camera::queryFrameWithHist()
{
	IplImage* frame = queryFrame();
	IplImage* gray = cvCreateImage(cvGetSize(frame), 8, 1);

	cvCvtColor(frame, gray, CV_RGB2GRAY);

	int boxHeight = getHeight() / 8;

	int bins = 256;
	int hsize[] = {bins};

	float xranges[] = { 0, 256 }; // grayscale 0 - 255
	float* ranges[] = { xranges };
 
	IplImage* planes[] = { gray };

	CvHistogram* hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges,1);
	cvCalcHist( planes, hist, 0, NULL);

	float max = 0;
	float min = 0;
	cvGetMinMaxHistValue(hist, &min, &max);

	// Draw histogram in lower right corner
	for(int i = 0; i < 256; i++) {
		float value = cvQueryHistValue_1D(hist, i);
		int normalized = cvRound(value*boxHeight/max);
		cvLine(
			frame, 
			cvPoint((getWidth()-256)+i, getHeight()), 
			cvPoint((getWidth()-256)+i, getHeight()-normalized), 
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

	return frame;
}

int Camera::getWidth() 
{
	int w = width;
	if(resizeWidth) {
		w = resizeWidth;
	}
	return w;
}

int Camera::getHeight()
{
	int h = height;
	if(resizeHeight) {
		h = resizeHeight;
	}
	return h;
}

} // end namespace
