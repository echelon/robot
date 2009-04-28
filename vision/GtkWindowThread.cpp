#include "GtkWindowThread.hpp"
#include "../internals/MainThreadControl.hpp"
#include <stdio.h>
#include <string.h>

namespace Vision {

GtkWindowThread::GtkWindowThread(int nImages, const char* wTitle):
	numImages(nImages),
	windowTitle(wTitle),
	imageList()
{
	// nothing
}

GtkWindowThread::~GtkWindowThread()
{
	// nothing
}

void GtkWindowThread::setup()
{
	// nothing
}

void GtkWindowThread::destroy()
{
	stop();
	Internals::MainThreadControl::signal();
}

void GtkWindowThread::execute(void*)
	{
	g_thread_init(0);
	gdk_threads_init();
    gtk_init(0, 0); // Omitting &argc, &argv

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), windowTitle);

	g_signal_connect(window, "destroy", G_CALLBACK(terminateCallback), 0);

	hbox = gtk_hbox_new(true, 0);

	for(int i = 0; i < numImages; i++) {
		GtkWidget* child = gtk_image_new_from_file("aas27893dff"); // non-existant file
		imageList.push_back(child);
		gtk_box_pack_start(GTK_BOX(hbox), child, true, true, 0);
	}

	gtk_container_add(GTK_CONTAINER(window), hbox);
	gtk_widget_show_all(window);

	// Main loop
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
}

void GtkWindowThread::showImage(IplImage* img, int panelId) 
{
	if(stopFlag) {
		return;
	}
	if(panelId > numImages-1 || panelId < 0) {
		printf("GtkWindowThread::showImage, panel number is out of bounds: %d/%d\n",
			panelId,
			numImages);
		return;
	}

	// TODO: Watch for memleaks
	GtkWidget* gtkImg = convertGtk(img);

	gdk_threads_enter();
	//gtk_container_remove(GTK_CONTAINER(hbox), imageList[panelId]);
	gtk_widget_destroy(imageList[panelId]);
	imageList[panelId] = gtkImg;

	gtk_box_pack_start(GTK_BOX(hbox), gtkImg, true, true, 0);
	gtk_box_reorder_child(GTK_BOX(hbox), gtkImg, panelId);

	gtk_widget_show_all(window);
	gdk_threads_leave();
}

// Adapted from:
// http://camus.arglabs.com/blog/archives/playing-with-opencv-in-gtk/capture-01tar
GtkWidget* GtkWindowThread::convertGtk(IplImage* img)
{
	/* // TODO TEST
	GdkPixbuf* pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, 8000, 8000);
	GtkWidget* gi = gtk_image_new_from_pixbuf(pb);
	return gi;*/

	IplImage* imgCopy;
	GtkWidget* gtkImg;
	GdkPixbuf* pix;

	if(!tempImg->imageData) {
		printf("RELEASE\n");
		//cvReleaseImage(&tempImg); // TODO: Causing segmentation faults. Why?
		// Must deallocate later to prevent memory leaks
	}

	// Must deallocate later to prevent memory leaks
	tempImg = cvCreateImage(
		cvSize(img->width,img->height), 
		img->depth, 
		img->nChannels);

	//cvCvtColor(img, tempImg, CV_BGR2RGB);
	cvCvtColor(img, tempImg, CV_BGR2RGB);


	/*guchar* gImageBuffer = 
		(guchar*) malloc(img->width*img->height*4*sizeof(guchar));
	guchar *gImagePtr = gImageBuffer;

	const guchar* iplImagePtr = (const guchar*)img->imageData;

	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			// We cannot help but copy manually.
				gImagePtr[0] = iplImagePtr[0];
				gImagePtr[1] = iplImagePtr[1];
				gImagePtr[2] = iplImagePtr[2];
				gImagePtr[3] = 0;

				gImagePtr += 4;
				iplImagePtr += 3;
		}
		iplImagePtr += img->widthStep-3*img->width;
	}

	// free gImageBuffer?*/

	pix = gdk_pixbuf_new_from_data(
		(guchar*)tempImg->imageData, // TODO: This is a major source of memleak
		GDK_COLORSPACE_RGB,
		FALSE,
		tempImg->depth,
		tempImg->width,
		tempImg->height,
		tempImg->widthStep,
		NULL,
		NULL);

	//free(gImageBuffer);

	gtkImg = gtk_image_new_from_pixbuf(pix);

	return gtkImg;
}


gboolean GtkWindowThread::terminateCallback(GtkWidget* widget, GdkEvent* event, void*)
{
	gtk_main_quit();
	return false;
}


} // end namespace
