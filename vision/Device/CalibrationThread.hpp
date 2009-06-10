#ifndef VISION_DEVICE_CALIBRATIONTHREAD_H
#define VISION_DEVICE_CALIBRATIONTHREAD_H

#include "../../internals/Thread.hpp"
#include <cv.h>

// forward decs
namespace Vision { class GtkWindowThread; }
namespace Vision { namespace Device { class Calibration; } }

namespace Vision {
namespace Device {

/**
 * GtkWindowThread is a thread that controls output through Gtk.
 */
class CalibrationThread: public Internals::Thread
{
	public:
		CalibrationThread(Calibration& cal);
		~CalibrationThread();

		/**
		 * Set the gui options.
		 */		
		void setWindow(GtkWindowThread* winThread, int winNumber);

	protected:
		/**
		 * Setup the thread.
		 * Virtual override, called automatically.
		 */
		//void setup();

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

		/**
		 * Gui Presentation
		 */
		Vision::GtkWindowThread* windowThread;
		int windowNumber;
		

};
}
}

#endif
