
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/GtkWindowThread.hpp"
#include "../../internals/MainThreadControl.hpp"
//#include "../../vision/Chessboard.hpp"


int main(int argc, char* argv[])
{
	Vision::Camera* cam1 = new Vision::Camera(1); // TODO: Lookup by manufacturer.
	Vision::Camera* cam2 = new Vision::Camera(2);
	
	cam1->setResize(320, 240); // 320x240. Orig 640x480
	cam2->setResize(320, 240);

	Vision::GtkWindowThread winThread(2, "OpenCV Stereoscopic Vision");
	winThread.start();

	IplImage* f1;
	IplImage* f2;

	printf("Test1\n");
	cam1->getCalibration();
	printf("Test2\n");
	//cam1->getCalibration()->calibrateThreaded();
	printf("Test3\n");
	//cam1->getCalibration().calibrateThreaded();


	/*Vision::Chessboard board1(cvSize(7,6), 10);
	Vision::Chessboard board2(cvSize(7,6));

	int skipFrames = 3;
	int frameCnt = 0;

	// Calibrate the camera.
	while(!board1.foundEnough()) {
		f1 = cam1->queryFrame();
		f2 = cam2->queryFrame();

		frameCnt = (frameCnt+1)%1000;
		if(frameCnt % skipFrames == 0) {
			board1.findAndDrawCorners(f1);
		}

		winThread.showImage(f1, 0);
		winThread.showImage(f2, 1);
	}
	printf("Found enough boards for cam 1\n");


	board1.calibrateCamPrep(f1);
	printf("Calibration done for cam 1\n");*/

	while(!Internals::MainThreadControl::wasSignaled()) {
		f1 = cam1->queryFrameWithHist(); // queryFrameWithHist()
		f2 = cam2->queryFrame();

		//IplImage* f1cal = board1.calibrateFrame(f1);

		//board1.findAndDrawCorners(f1);
		//board2.findAndDrawCorners(f2);

		winThread.showImage(f1, 0);
		//winThread.showImage(f1cal, 1);
		winThread.showImage(f2, 1);
		
		//win1->showImageWithHist(f1);
		//win2->showImageWithHist(f2);

		//cvReleaseImage(&f1);
		//cvReleaseImage(&f2);

	}
	return 0;
}
