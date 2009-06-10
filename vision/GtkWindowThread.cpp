#include "GtkWindowThread.hpp"
#include "../internals/MainThreadControl.hpp"
#include <stdio.h>

namespace Vision {

GtkWindowThread::GtkWindowThread(int nImages, const char* wTitle):
	numImages(nImages),
	numRows(1),
	windowTitle(wTitle),
	imageList()
{
	// nothing
}

GtkWindowThread::GtkWindowThread(int nImages, int nRows, const char* wTitle):
	numImages(nImages),
	numRows(nRows),
	windowTitle(wTitle),
	imageList()
{
	if(nRows < 1) {
		printf("Cannot use less than 1 row\n");
		// TODO: exception
	}
	if(nImages%nRows != 0) {
		printf("Uneven number of images per row\n");
		// TODO: possible exception.
	}
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

	int numCols = numImages/numRows;
	printf("Num rows/cols: %d/%d\n", numRows, numCols);

	vbox = gtk_vbox_new(true, 0);

	// Create rows, cols
	int imgCnt = 0;
	for(int i = 0; i < numRows; i++) {
		GtkWidget* hhbox = gtk_hbox_new(true, 0);
		gtk_box_pack_start(GTK_BOX(vbox), hhbox, true, true, 0);
		for(int j = 0; j < numCols && imgCnt < numImages; j++) {
			GtkWidget* child = gtk_image_new_from_file("aas27893dff"); // non-existant file
			imageList.push_back(child);
			gtk_box_pack_start(GTK_BOX(hhbox), child, true, true, 0);
			imgCnt++;
		}
	}

	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);

	// Main loop
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
}

void GtkWindowThread::showImage(IplImage* img, int panelId) 
{
	GdkPixbuf* pix;
	GdkPixbuf* oldPix;

	if(stopFlag) {
		return;
	}
	if(panelId > numImages-1 || panelId < 0) {
		printf("GtkWindowThread::showImage, panel number is out of bounds: %d/%d\n",
			panelId,
			numImages);
		return;
	}

	if(!img) {
		return;
	}

	pix = convertPixbuf(img);

	gdk_threads_enter();

	// Delete old pixbuf and replace (TODO: fix error on initial image type)
	oldPix = gtk_image_get_pixbuf(GTK_IMAGE(imageList[panelId]));
	gtk_image_clear(GTK_IMAGE(imageList[panelId]));

	gtk_image_set_from_pixbuf(GTK_IMAGE(imageList[panelId]), pix);

	if(oldPix) {
		g_object_unref(G_OBJECT(oldPix)); // fixes memleak
	}

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
	delete[] pixels; // prevent memleak
}


} // end namespace
