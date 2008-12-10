#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

#include "gui/Trackbar.hpp"
#include "transform/hough.hpp"
#include "transform/overlay.hpp"

/**
 * Main 
 */
int main(int argc, char** argv)
{
	CvCapture* capture = cvCreateCameraCapture(0);
	if (!capture) {
        fprintf(stderr,"Could not initialize capturing. Exiting...\n");
        return -1;
	}

	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240); 

	IplImage* src = 0;
	IplImage* gray = 0;
	IplImage* dst = 0;
	IplImage* hough = 0;

    cvNamedWindow("output", 1);
	Trackbar t1 = Trackbar("output", "thresh1", 100, 50);
	Trackbar t2 = Trackbar("output", "thresh2", 200, 80);

	while(1)
    {
		src = cvQueryFrame(capture);
		if (!src) {
			break;
		}
		
		if (!dst) {
			printf("Creating images...\n");
			dst = cvCreateImage(cvGetSize(src), 8, 1);
			gray = cvCreateImage(cvGetSize(src), 8, 1);
			hough = cvCreateImage(cvGetSize(src), 8, 3);
		}

		cvCvtColor(src, gray, CV_BGR2GRAY); // grayscale original

        cvCanny(gray, dst, t1.value, t2.value, 3); // grayscale canny
        //cvCanny(gray, dst, 50, 80, 3); // grayscale canny

		IplImage * overlay = overlayEdges(dst, src);

        //cvCvtColor(dst, hough, CV_GRAY2BGR); // gs canny color canny
		houghLines(dst, overlay); // bw canny -> color hough

        //cvShowImage("output", src);
        cvShowImage("output", overlay);

        if((cvWaitKey(10) & 255) == 27) break;

    }

    cvReleaseCapture(&capture);    
    cvDestroyAllWindows();
	return 0;
}
