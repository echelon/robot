#ifndef VISION_DEVICE_CALIBRATIONTHREAD_H
#define VISION_DEVICE_CALIBRATIONTHREAD_H

#include "../../internals/Thread.hpp"
#include <cv.h>

namespace Vision {
namespace Device {

// forward decs
class Calibration;

/**
 * GtkWindowThread is a thread that controls output through Gtk.
 */
class CalibrationThread: public Internals::Thread
{
	public:
		CalibrationThread(Calibration& cal);
		~CalibrationThread();


	protected:
		/**
		 * Setup the thread.
		 * Virtual override, called automatically.
		 */
		void setup();

		/**
		 * "Destroy" the thread.
		 * Virtual override, called automatically.
		 */
		void destroy();

		/**
		 * Thread's main execution
		 */
		void execute(void*);

	private:
		Calibration& calibration;

};
}
}

#endif
