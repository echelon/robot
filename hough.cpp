#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

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
	IplImage* dstColor = 0;

    cvNamedWindow("window1", 1);
    cvNamedWindow("window2", 1);

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
			dstColor = cvCreateImage(cvGetSize(src), 8, 3);
		}

		cvCvtColor(src, gray, CV_BGR2GRAY);
        cvCanny(gray, dst, 50, 200, 3);

		// Begin hough
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* lines = 0;

        cvCvtColor(dst, dstColor, CV_GRAY2BGR);

        lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10);
        for(int i = 0; i < lines->total; i++ )
        {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            cvLine( dstColor, line[0], line[1], CV_RGB(255,0,0), 3, 8);
        }
		// End hough

        cvShowImage("window1", src);
        cvShowImage("window2", dstColor);
        int c = cvWaitKey(10);
        if((char)c == 27) break;
    }

    cvReleaseCapture(&capture);    
    cvDestroyWindow("window1");
	return 0;
}

