#include "CalibrationThread.hpp"
#include "Calibration.hpp"
#include "../Camera.hpp"
#include <stdio.h>

namespace Vision {
namespace Device {

CalibrationThread::CalibrationThread(Calibration& cal):
	calibration(cal)
{
	// nothing
}

CalibrationThread::~CalibrationThread()
{
	// nothing
}

void CalibrationThread::setup()
{
	// nothing
}

void CalibrationThread::destroy()
{
	// nothing
}

void CalibrationThread::execute(void*)
{
	/*IplImage* frame;
	int frameCnt = 0;

	printf("Beginning calibration\n");

	while(!calibration.foundEnough()) {
		frame = calibration.getCamera().queryFrame();
		frameCnt = (frameCnt+1)%1000;
		if(frameCnt % calibration.getSkipCount() == 0) {
			calibration.findCorners(frame); // TODO: findCorners() only.
		}
	}
	printf("Calibration data collected.\n");

	calibration.finalize(frame);

	printf("Calibration finished... ending thread\n");*/
}

} // end namespace Device
} // end namespace Vision
