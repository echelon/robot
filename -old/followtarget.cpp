#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

#include "gui/Trackbar.hpp"
#include "imagelib/colordim.hpp"
#include "imagelib/colorreplace.hpp"
#include "imagelib/pixeldensity.hpp"
#include "serial/Serializer.hpp"
#include "tts/Festival.hpp"

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

	Festival f = Festival();
	
	f.say("Initializing Follow Target program.");

	IplImage* src = 0;
	IplImage* dst = 0;

    cvNamedWindow("output", 1);
	Trackbar t1 = Trackbar("output", "threshold", 255, 25);
	Trackbar t2 = Trackbar("output", "percent", 255, 30);

	while(1)
    {
		src = cvQueryFrame(capture);
		if (!src) {
			break;
		}
		
		if (!dst) {
			dst = cvCreateImage(cvGetSize(src), 8, 3);
		}

		int colors1[5][7] = {
			{145, 133, 191, 0, 255, 0, t1.value},	// Purple tape
			{116, 103, 149, 0, 255, 0, t1.value},	// Purple tape
			{74,  168,  26, 255, 0, 0, t1.value},	// Green tape under yellow light
			{89,   98, 103, 255, 0, 0, t1.value},	// Blue tape under yellow light 1
			{82,   83,  98, 255, 0, 0, t1.value}	// Blue tape under yellow light 2
		};
		int colors2[3][3] = {
			//{145, 133, 191},	// Purple tape
			//{116, 103, 149},	// Purple tape
			{74,  168,  26},	// Green tape under yellow light
			{89,   98, 103},	// Blue tape under yellow light 1
			{82,   83,  98}		// Blue tape under yellow light 2
		};

		int others[3] = {0, 0, 0};
		int red[3] = {255, 0, 0};

		//dst = dimOtherColors(src, colors, t1.value, 0.0f);
		dst = replaceColors(src, colors1, others);

		float percent = (t2.value*0.1f);
		if(densityGt(dst, red, percent)) {
			f.say("Object located");
		}

        cvShowImage("output", dst);

        if((cvWaitKey(10) & 255) == 27) break;

    }

    cvReleaseCapture(&capture);    
    cvDestroyAllWindows();
	return 0;
}
