#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

/** 
 * houghLines
 * Needs an edge-detected image, such as one from Canny, as the
 * source image. 
 */
void houghLines(IplImage* src, IplImage* dst)
{
		// Copy image to a destination image
		//IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
        //cvCvtColor(src, dst, CV_GRAY2BGR);

        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* lines = 0;

        lines = cvHoughLines2(src, storage, CV_HOUGH_PROBABILISTIC, 
					1, CV_PI/180, 50, 50, 10);
        for(int i = 0; i < lines->total; i++) {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            cvLine(dst, line[0], line[1], CV_RGB(255,0,0), 3, 8);
        }
}

/**
 * extractChannel
 * Extract a single image channel into another multichannel image.
 */
IplImage* extractChannel(IplImage* src, int channel)
{
	int height     = src->height;
	int width      = src->width;
	int step       = src->widthStep/sizeof(uchar);
	int channels   = src->nChannels;
	uchar* sdata    = (uchar*)src->imageData;

	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	uchar* dstdata    = (uchar*)src->imageData;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int addr = i*step+j*channels+channel;
			dstdata[addr] = sdata[addr];
		}
	}
	return dst;
}

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

	int thresh1 = 50;
	int thresh2 = 80;

	cvCreateTrackbar("thresh1", "output", &thresh1, 100, NULL);
	cvCreateTrackbar("thresh2", "output", &thresh2, 200, NULL);


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


		int thresh1 = cvGetTrackbarPos("thresh1", "output");
		int thresh2 = cvGetTrackbarPos("thresh2", "output");
        cvCanny(gray, dst, thresh1, thresh2, 3); // grayscale canny
        //cvCanny(gray, dst, 50, 80, 3); // grayscale canny

        //cvCvtColor(dst, hough, CV_GRAY2BGR); // gs canny color canny
		houghLines(dst, dst); // bw canny -> color hough

        //cvShowImage("output", src);
        cvShowImage("output", dst);

        if((cvWaitKey(10) & 255) == 27) break;

    }

    cvReleaseCapture(&capture);    
    cvDestroyAllWindows();
	return 0;
}
