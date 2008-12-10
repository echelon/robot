#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

#include "gui/Trackbar.hpp"
#include "imagelib/dim.hpp"
#include "imagelib/hough.hpp"
#include "imagelib/overlay.hpp"
#include "imagelib/sepchannel.hpp"

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
	Trackbar t1 = Trackbar("output", "v1", 255, 100);
	Trackbar t2 = Trackbar("output", "v2", 255, 150);

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

		//cvCvtColor(src, gray, CV_BGR2GRAY);
        //cvCanny(gray, dst, t1.value, t2.value, 3);
		//IplImage * overlay = overlayEdges(dst, src);
		//houghLines(dst, overlay); 


		int color[3] = {95, 202, 65};

		IplImage* wh = dimOtherColors(src, color, t1.value, (t2.value*0.01f));

		float percents[3] = {0.37254902f, 0.792156863f, 0.254901961f};
		//float percents[3] = {1.0f, 1.0f, 1.0f};
		//float percents[3] = {0.1f, 0.3f, 0.1f};
		IplImage* out = sepChannel(wh, percents);




		//cvCvtColor(out, gray, CV_BGR2GRAY);
		//cvThreshold(gray, dst, t1.value, t2.value, CV_THRESH_BINARY);


        cvShowImage("output", wh);

        if((cvWaitKey(10) & 255) == 27) break;

    }

    cvReleaseCapture(&capture);    
    cvDestroyAllWindows();
	return 0;
}
