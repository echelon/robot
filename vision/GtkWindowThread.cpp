#include "GtkWindowThread.hpp"
#include "../internals/MainThreadControl.hpp"
#include <stdio.h>
#include <string.h>
#include <string>

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
	GdkPixbuf* pix = convertPixbuf(img);

	gdk_threads_enter();
	//gtk_container_remove(GTK_CONTAINER(hbox), imageList[panelId]);

	// Delete pixbuf
	GdkPixbuf* old = gtk_image_get_pixbuf(GTK_IMAGE(imageList[panelId]));
	gtk_image_clear(GTK_IMAGE(imageList[panelId]));

	gtk_image_set_from_pixbuf(GTK_IMAGE(imageList[panelId]), pix);
	g_object_unref(G_OBJECT(old)); 


	//gtk_widget_destroy(imageList[panelId]);
	//imageList[panelId] = gtkImg;

	//gtk_box_pack_start(GTK_BOX(hbox), gtkImg, true, true, 0);
	//gtk_box_reorder_child(GTK_BOX(hbox), gtkImg, panelId);

	gtk_widget_show_all(window);
	gdk_threads_leave();
}

GdkPixbuf* GtkWindowThread::convertPixbuf(IplImage* img)
{

	unsigned char* buffer;
	GdkPixbuf* pix;

	buffer = copyImageData(img);

	pix = gdk_pixbuf_new_from_data(
		(guchar*)buffer, // TODO: This is a major source of memleak
		GDK_COLORSPACE_RGB,
		FALSE,
		img->depth,
		img->width,
		img->height,
		img->widthStep,
		(GdkPixbufDestroyNotify)pixbufDestroyCallback,
		NULL);

	return pix;
}


// Adapted from:
// http://camus.arglabs.com/blog/archives/playing-with-opencv-in-gtk/capture-01tar
GtkWidget* GtkWindowThread::convertGtk(IplImage* img)
{

	unsigned char* buffer;
	GdkPixbuf* pix;
	GtkWidget* gtkImg;

	buffer = copyImageData(img);

	pix = gdk_pixbuf_new_from_data(
		(guchar*)buffer, // TODO: This is a major source of memleak
		GDK_COLORSPACE_RGB,
		FALSE,
		img->depth,
		img->width,
		img->height,
		img->widthStep,
		(GdkPixbufDestroyNotify)pixbufDestroyCallback,
		NULL);

	gtkImg = gtk_image_new_from_pixbuf(pix);
	return gtkImg;
}

unsigned char* GtkWindowThread::copyImageData(IplImage* img)
{
	if(!img) {
		printf("GtkWindowThread::copyImageData, No image supplied.\n");
		return 0;
	}

	unsigned char *buffer,*temp1,*temp2;
	IplImage* imgCopy;

	imgCopy = cvCreateImage(
		cvSize(img->width,img->height), 
		img->depth, 
		img->nChannels);

	cvCvtColor(img, imgCopy, CV_BGR2RGB);

	// Solution from:
	// http://chi3x10.wordpress.com/2008/05/07/be-aware-of-memory...
	// -alignment-of-iplimage-in-opencv/
	buffer = 
		new unsigned char[imgCopy->width*imgCopy->height*imgCopy->nChannels];
	temp2 = buffer;

	temp1 = (unsigned char*) imgCopy->imageData;

	for(int i=0;i<imgCopy->height;i++) {
		memcpy(temp2, temp1, imgCopy->width*imgCopy->nChannels);

		// jump to next line
		temp2 = temp2 + imgCopy->widthStep;
		temp1 = temp1+ imgCopy->width*imgCopy->nChannels;
	}

	cvReleaseImage(&imgCopy);

	return buffer;
}


gboolean GtkWindowThread::terminateCallback(GtkWidget* widget, GdkEvent* event, void*)
{
	gtk_main_quit();
	return false;
}

void GtkWindowThread::pixbufDestroyCallback(guchar* pixels, gpointer data)
{
	printf("Free callback\n");
	delete[] pixels;
}


} // end namespace
