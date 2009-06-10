#include "CalibrationThread.hpp"
#include "Calibration.hpp"
#include "../Camera.hpp"
#include "../GtkWindowThread.hpp"
#include <stdio.h>

namespace Vision {
namespace Device {

CalibrationThread::CalibrationThread(Calibration& cal):
	calibration(cal),
	windowThread(0),
	windowNumber(0)
{
	// nothing
}

CalibrationThread::~CalibrationThread()
{
	// nothing
}

/*void CalibrationThread::setup()
{
	// nothing
}*/

void CalibrationThread::destroy()
{
	// nothing
}

void CalibrationThread::execute(void*)
{
	IplImage* frame;
	Camera& cam = calibration.getCamera();
	int frameCnt = 0;

	int skipCount = calibration.getSkipCount();
	if(skipCount < 1) {
		skipCount = 1;
	}

	while(!calibration.foundEnough()) {
		frame = cam.queryFrame();

		frameCnt = (frameCnt+1)%1000;
		if(frameCnt % skipCount == 0) {
			calibration.findAndDrawCorners(frame); // TODO: findCorners() only.
		}

		if(windowThread) {
			windowThread->showImage(frame, windowNumber);
		}
	}
	printf("Calibration data collected from this camera.\n");

	calibration.finalize(frame);

	printf("Calibration finished...\n");
}

void CalibrationThread::setWindow(GtkWindowThread* winThread, int winNumber)
{
	windowThread = winThread;
	windowNumber = winNumber;
}

} // end namespace Device
} // end namespace Vision
