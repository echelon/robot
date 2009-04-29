#ifndef VISION_GtkWindowThread_H
#define VISION_GtkWindowThread_H

#include "../internals/Thread.hpp"
#include <gtk/gtk.h>
#include <cv.h>
#include <vector>

namespace Vision {
/**
 * GtkWindowThread is a thread that controls output through Gtk.
 */
class GtkWindowThread: public Internals::Thread
{
	public:
		GtkWindowThread(int nImages = 1, const char* wTitle = "Untitled Window");

		~GtkWindowThread();

		/**
		 * Show an OpenCV image in the specified panel.
		 */
		void showImage(IplImage* img, int panelId = 0);

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

		/**
		 * Callback when Gtk session ended.
		 */
		static gboolean terminateCallback(GtkWidget* widget, GdkEvent* event, void*);

		/**
		 * Callback when pixbufs are zero-referenced.
		 */
		static void pixbufDestroyCallback(guchar *pixels, gpointer data);

	private:
		int numImages;
		const char* windowTitle;

		GtkWidget* window;
		GtkWidget* hbox;

		std::vector<GtkWidget*> imageList;

		/**
		 * Convert OpenCV image to Gtk pixbuf.
		 * Adapted from internet sources (listed in the cpp file)
		 */
		GtkWidget* convertGtk(IplImage* img);
		GdkPixbuf* convertPixbuf(IplImage* img);


		/**
		 * Copy the IplImage's imageData field and return it.
		 * Memory must be freed by caller.
		 */
		unsigned char* copyImageData(IplImage* img);
};
}

#endif
