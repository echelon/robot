
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/Device/Calibration.hpp" // TODO: Hide
#include "../../vision/GtkWindowThread.hpp"
#include "../../internals/MainThreadControl.hpp"


int main(int argc, char* argv[])
{
	printf("Stereo Project begin...\n\n");

	Vision::Camera* cam1 = new Vision::Camera(1); // TODO: Lookup by manufacturer.
	Vision::Camera* cam2 = new Vision::Camera(2);
	
	cam1->setResize(320, 240); // 320x240. Orig 640x480
	cam2->setResize(320, 240);

	Vision::GtkWindowThread* winThread = new Vision::GtkWindowThread(2, "OpenCV Stereoscopic Vision");
	winThread->start();

	IplImage* f1;
	IplImage* f2;

	Vision::Device::Calibration* cc1;
	Vision::Device::Calibration* cc2;

	cc1 = cam1->getCalibration();
	cc2 = cam2->getCalibration();

	cc1->setWindow(winThread, 0);
	cc2->setWindow(winThread, 1);

	cc1->calibrateThreaded();
	cc2->calibrateThreaded();

	cc1->setWindow(winThread, 0);
	cc2->setWindow(winThread, 1);

	// Wait for calibration...
	while(!Internals::MainThreadControl::wasSignaled() &&
		 (!cc1->isCalibrated() || !cc2->isCalibrated()));

	// Calibration finished, Main Functionality
	while(!Internals::MainThreadControl::wasSignaled() ) { 
		f1 = cam1->queryFrame(true);
		f2 = cam1->queryFrame(false);
		winThread->showImage(f1, 0);
		winThread->showImage(f2, 1);
	}


	while(!Internals::MainThreadControl::wasSignaled()) {
		f1 = cam1->queryFrameWithHist(); // queryFrameWithHist()
		f2 = cam2->queryFrame();

		//IplImage* f1cal = board1.calibrateFrame(f1);

		//board1.findAndDrawCorners(f1);
		//board2.findAndDrawCorners(f2);

		winThread->showImage(f1, 0);
		//winThread.showImage(f1cal, 1);
		winThread->showImage(f2, 1);
		
		//win1->showImageWithHist(f1);
		//win2->showImageWithHist(f2);

		//cvReleaseImage(&f1);
		//cvReleaseImage(&f2);

	}
	return 0;
}
