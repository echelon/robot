#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

#include "ai/Map.hpp"
#include "gui/Trackbar.hpp"
#include "imagelib/colordim.hpp"
#include "imagelib/colorreplace.hpp"
#include "imagelib/hough.hpp"
#include "imagelib/overlay.hpp"
#include "imagelib/sepchannel.hpp"
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

	//Serializer a = Serializer();
	//a.mogo(50, 50);

	// *** BEGIN TTS ****
    /*FILE *iopipe;

    if( (iopipe = popen( "festival --pipe", "w" )) == NULL )
        exit( 1 );


	fputs("(SayText \"Testing 1 2 3\")", iopipe);
	fflush(iopipe);*/

	Festival f = Festival();

	Map map = Map(100, 100);
	
	//f.say("test");
	//f.say("Testing text to speech, 1 2 3");

	char buff1[200];
	sprintf(buff1, "Current position is at %i/%i, facing up, with a velocity of 50.", map.getCurrentPos().x, map.getCurrentPos().y);
	f.say(buff1);

	//printf(buff1);

    //printf( "\nProcess returned %d\n", pclose( iopipe ) );
	// *** END TTS ****

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
		
		char buff[200];
		sprintf(buff, "Current position is at %i-%i, facing up, with a velocity of 50.", map.getCurrentPos().x, map.getCurrentPos().y);
		f.say(buff);

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


		//int color[3] = {95, 202, 65}; // Green tape
		//int color[3] = {68, 137, 230}; // Blue tape
		//int color[3] = {145, 133, 191}; // Purple tape
		//int color[3] = {116, 103, 149}; // Purple tape

		//int color[3] = {176, 144, 103}; // Light brick
		//int color[3] = {197, 158, 111}; // Dark brick
		//int color[3] = {132, 100, 65}; // Dark grout

		//IplImage* wh = dimOtherColors(src, color, t1.value, (t2.value*0.01f));


		int colors[4][7] = {
			{176, 144, 103, 255, 0, 0, t1.value}, // Light brick
			{197, 158, 158, 255, 0, 0, t1.value}, // Dark brick
			{132, 100, 65,  255, 0, 0, t1.value}, // Dark grout
			{116, 103, 149, 0, 255, 0, t2.value}  // Purple tape
		};

		int others[3] = {0, 0, 0};

		IplImage* wh = replaceColors(src, colors, others);

		if(t1.value == 0) {
			f.say("Threshold at 0");
		}

		//float percents[3] = {0.37254902f, 0.792156863f, 0.254901961f};
		//float percents[3] = {1.0f, 1.0f, 1.0f};
		//float percents[3] = {0.1f, 0.3f, 0.1f};
		//IplImage* out = sepChannel(wh, percents);


		//cvCvtColor(out, gray, CV_BGR2GRAY);
		//cvThreshold(gray, dst, t1.value, t2.value, CV_THRESH_BINARY);


        cvShowImage("output", wh);

        if((cvWaitKey(10) & 255) == 27) break;

    }

    cvReleaseCapture(&capture);    
    cvDestroyAllWindows();
	return 0;
}
