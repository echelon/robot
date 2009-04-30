
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/Window.hpp"
#include "../../vision/GtkWindowThread.hpp"
#include "../../internals/MainThreadControl.hpp"
#include "../../vision/Chessboard.hpp"


int main(int argc, char* argv[])
{
	//Vision::Window* win1 = new Vision::Window("win1");
	//Vision::Window* win2 = new Vision::Window("win2");

	Vision::Camera* cam1 = new Vision::Camera(1); // TODO: Lookup by manufacturer.
	Vision::Camera* cam2 = new Vision::Camera(2);
	
	cam1->setResize(320, 240); // 320,240. Orig 640x480
	cam2->setResize(320, 240);

	Vision::GtkWindowThread winThread(2, "OpenCV Stereoscopic Vision");
	winThread.start();


	/*Internals::Registry::add("window", window);
	Internals::Registry::add("window2", window2);
	Internals::Registry::add("camera", camera);

	camera->setResize(320, 240);*/

	IplImage* f1;
	IplImage* f2;

	Vision::Chessboard board1(cvSize(7,7));
	Vision::Chessboard board2(cvSize(7,7));

	while(!Internals::MainThreadControl::wasSignaled()) {
		f1 = cam1->queryFrame(); // queryFrameWithHist()
		f2 = cam2->queryFrame();

		//board1.findAndDrawCorners(f1);
		//board2.findAndDrawCorners(f2);

		winThread.showImage(f1, 0);
		winThread.showImage(f2, 1);
		
		//win1->showImageWithHist(f1);
		//win2->showImageWithHist(f2);

		//cvReleaseImage(&f1);
		//cvReleaseImage(&f2);

		//if(Vision::Window::keypress())
		//	break;
	}
	return 0;
}
