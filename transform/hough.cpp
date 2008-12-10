#include "hough.hpp"

void houghLines(IplImage* src, IplImage* dst)
{
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* lines = 0;

        lines = cvHoughLines2(src, storage, CV_HOUGH_PROBABILISTIC, 
					1, CV_PI/180, 50, 50, 10);
        for(int i = 0; i < lines->total; i++) {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            cvLine(dst, line[0], line[1], CV_RGB(255,0,0), 3, 8);
        }
}
