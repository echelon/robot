/**
 * Robot Laser following
 * The robot will follow a green laser pointer.
 */

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include "../../internals/Registry.hpp"
#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/Window.hpp"

IplImage* getLaserFromColor(IplImage* src)
{
	// Constants for testing
	bool preserve = false;
	int thresh = 25;

	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1); // TODO: Was 3 channel
	IplImage* gauss = cvCloneImage(src);
	
	cvSmooth(src, gauss);

	Vision::RgbImage first(gauss); 
	Vision::RgbImage second(src);

	Vision::RgbImage s(src);
	Vision::BwImage d(dst);

	bool foundLaser = false;
	
	// Try finding in gaussian
	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			if(preserve) {
				d[i][j] = first[i][j].r/4;
				/*d[i][j].g = first[i][j].g/4;
				d[i][j].b = first[i][j].b/4;*/
			}

			int rdif = first[i][j].g - first[i][j].r;
			int bdif = first[i][j].g - first[i][j].b;
			if(rdif > thresh && bdif > thresh) {
				d[i][j] = 255;
				foundLaser = true;
			}
		}
	}

	cvReleaseImage(&gauss);
	
	if (foundLaser) {
		return dst;
	}

	// Try finding in original image
	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			if(preserve) {
				d[i][j] = second[i][j].r/2;
				/*d[i][j].g = second[i][j].g/2;
				d[i][j].b = second[i][j].b/2;*/
			}

			int rdif = second[i][j].g - second[i][j].r;
			int bdif = second[i][j].g - second[i][j].b;
			if(rdif > thresh && bdif > thresh) {
				d[i][j] = 255;
			}
		}
	}
	return dst;
}


IplImage* getLaserFromLuma(IplImage* src)
{
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	IplImage* srcGray = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* dstGray = cvCreateImage(cvGetSize(src), 8, 1);

	cvCvtColor(src, srcGray, CV_RGB2GRAY);

	Vision::RgbImage s(src); 
	Vision::RgbImage d(dst);

	Vision::BwImage sg(srcGray);
	Vision::BwImage dg(dstGray);

	bool foundLaser = false;

	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {

			double factor = 1.2; // higher values = darker
			int val = (int)(log((double)sg[i][j])/log(factor));
			sg[i][j] = val%255;
		}
	}

	if(1) {
		cvEqualizeHist(srcGray, dstGray);
		return dstGray;
	}

	return srcGray;
}

/**
 * Verify if there's a large enough area to be considered a laser.
 */
bool verifyNeighbors(IplImage* image, int x, int y)
{
	int pix = 0;
	Vision::BwImage img(image);

	for (int i = y - 5; i < y + 5; i++) {
		for (int j = x - 5; j < x + 5; j++) {
			if(i < 0 && i >= image->height) { 
				continue;
			}
			if(j < 0 && j >= image->width) { 
				continue;
			}
			if(img[i][j] == 255) {
				pix += 1;
			}
		}
	}
	if(pix > 5) {
		return true;
	}
	return false;
}

/**
 * Get the laser
 * Releases IplImage* src
 */
void getLaser(IplImage* src)
{
	bool foundLaser = false;

	IplImage* search = getLaserFromColor(src);
	Vision::BwImage s(search);
	
	IplImage* erode;
	
	printf("Test1\n");
	IplConvKernel* kern = 
		cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_CROSS);
	
	printf("Test2\n");
	cvErode(search, erode, 0);
	printf("Test3\n");

	bool found = false;
	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			int val = s[i][j];
			
			if(val == 255 && verifyNeighbors(search, j, i)) {
				printf("Found at %dx%d\n", j, i);
				found = true;
				break;
			}
		}
	}
	
	// Show output
	Vision::Window* window = 
			(Vision::Window*) Internals::Registry::get("window");
	window->showImageWithHist(search);
	
	
	Vision::Window* window2 = 
			(Vision::Window*) Internals::Registry::get("window2");
	window2->showImageWithHist(erode);
	
	cvReleaseImage(&search);
}


/** 
 * Main Function
 * Set up the robot to follow a laser pointer.
 */

int main(int argc, char** argv)
{
	Vision::Window* window = new Vision::Window("main");
	Vision::Window* window2 = new Vision::Window("other");
	Vision::Camera* camera = new Vision::Camera();
	
	Internals::Registry::add("window", window);
	Internals::Registry::add("window2", window2);
	Internals::Registry::add("camera", camera);

	camera->setResize(320, 240);

	IplImage* frame;
	IplImage* laser;

	while(1) {
		frame = camera->queryFrame();

		getLaser(frame);
		
		cvReleaseImage(&frame);
		
		if(Vision::Window::keypress())
			break;
	}
	
	delete camera;
	delete window;

	return 0;
}
