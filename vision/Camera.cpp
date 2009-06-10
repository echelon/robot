#include "Camera.hpp"
#include "Canvas.hpp"
#include <stdio.h>
#include <stdexcept>

namespace Vision {

Camera::Camera(int device):
	resizeWidth(0),
	resizeHeight(0),
	queryFrameBuff(0),
	calibratedBuff(0),
	deviceNum(device)
{
	capture = cvCreateCameraCapture(device);
	if (!capture) {
		// TODO: Throw error
	}

	width  = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);

	calibration = new Device::Calibration(this, cvSize(7,7), 12);
	calibration->setSkipCount(4);
}

Camera::~Camera()
{
	cvReleaseCapture(&capture);
	delete calibration;
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

/**
 * Do not deallocate the IplImage* returned. 
 * The image is not owned by caller.
 */
IplImage* Camera::queryFrame(bool returnCalibrated)
{
	IplImage* frame;

	frame = cvQueryFrame(capture); // capture device deallocates on call!!

	/*while(frame == 0) {
		frame = cvQueryFrame(capture);
	}*/

	// if setResize()
	if(resizeWidth && resizeHeight) {
		if(queryFrameBuff != 0) {
			// cannot deallocate frame from cvQueryFrame()!
			cvReleaseImage(&queryFrameBuff);
		}

		queryFrameBuff = 
			cvCreateImage(cvSize(resizeWidth, resizeHeight), IPL_DEPTH_8U, 3);
		cvResize(frame, queryFrameBuff);

		if(!calibration->isCalibrated() || !returnCalibrated) {
			return queryFrameBuff;
		}
	}

	// if calibrated
	if(calibration->isCalibrated() && returnCalibrated) {
		if(calibratedBuff != 0) {
			cvReleaseImage(&calibratedBuff);
		}

		if(resizeWidth && resizeHeight) {
			calibratedBuff = cvCloneImage(queryFrameBuff);
			cvRemap(calibratedBuff, queryFrameBuff, 
				calibration->getXMap(), calibration->getYMap());
		}
		else {
			calibratedBuff = cvCloneImage(frame);
			cvRemap(calibratedBuff, frame, 
				calibration->getXMap(), calibration->getYMap());
		}
		return calibratedBuff;
	}
	return frame;
}

IplImage* Camera::queryFrameWithHist(bool returnCalibrated)
{
	IplImage* frame = queryFrame(returnCalibrated);
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
	}

	cvReleaseImage(&gray);
	cvReleaseHist(&hist);

	return frame;
}

int Camera::getWidth() 
{
	if(!resizeWidth) {
		return width;
	}
	return resizeWidth;
}

int Camera::getHeight()
{
	if(!resizeHeight) {
		return height;
	}
	return resizeHeight;
}

Device::Calibration* Camera::getCalibration()
{
	if(calibration == 0) {
		printf("No calibration exists!\n");
		return 0;
	}
	printf("Got calibration...\n");
	return calibration;
}

bool Camera::saveConfig(const char* filename)
{
	return calibration->save(filename);
}

bool Camera::loadConfig(const char* filename)
{
	return calibration->load(filename);
}

} // end namespace
