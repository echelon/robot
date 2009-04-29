
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/Window.hpp"
#include "../../vision/GtkWindowThread.hpp"
#include "../../internals/MainThreadControl.hpp"


int main(int argc, char* argv[])
{
	//Vision::Window* win1 = new Vision::Window("win1");
	//Vision::Window* win2 = new Vision::Window("win2");

	Vision::Camera* cam1 = new Vision::Camera(0); // TODO: Lookup by manufacturer.
	Vision::Camera* cam2 = new Vision::Camera(1);
	
	cam1->setResize(320, 240);
	cam2->setResize(320, 240);

	Vision::GtkWindowThread winThread(2, "OpenCV Stereoscopic Vision");
	winThread.start();


	/*Internals::Registry::add("window", window);
	Internals::Registry::add("window2", window2);
	Internals::Registry::add("camera", camera);

	camera->setResize(320, 240);*/

	IplImage* f1;
	IplImage* f2;

	while(!Internals::MainThreadControl::wasSignaled()) {
		f1 = cam1->queryFrame(); // queryFrameWithHist()
		f2 = cam2->queryFrame();

		winThread.showImage(f1, 0);
		winThread.showImage(f2, 1);
		
		//win1->showImageWithHist(f1);
		//win2->showImageWithHist(f2);

		cvReleaseImage(&f1);
		cvReleaseImage(&f2);

		//if(Vision::Window::keypress())
		//	break;
	}
	return 0;
}
