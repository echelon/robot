#include <math.h>
#include "dim.hpp"

IplImage* dimColor(IplImage* src, int rgb[3], int threshold, float percent)
{
	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - rgb[0]);
			int d2 = abs(imgDst[i][j].g - rgb[1]);
			int d3 = abs(imgDst[i][j].b - rgb[2]);

			if(d1 < threshold && d2 < threshold && d3 < threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
	return dst;
}

void dimColor(IplImage* src, IplImage* dst, int rgb[3], int threshold, float percent)
{
	dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - rgb[0]);
			int d2 = abs(imgDst[i][j].g - rgb[1]);
			int d3 = abs(imgDst[i][j].b - rgb[2]);

			if(d1 < threshold && d2 < threshold && d3 < threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
}

IplImage* dimOtherColors(IplImage* src, int rgb[3], int threshold, float percent)
{
	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - rgb[0]);
			int d2 = abs(imgDst[i][j].g - rgb[1]);
			int d3 = abs(imgDst[i][j].b - rgb[2]);

			if(d1 > threshold || d2 > threshold || d3 > threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
	return dst;
}

void dimOtherColors(IplImage* src, IplImage* dst, int rgb[3], int threshold, float percent)
{
	dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - rgb[0]);
			int d2 = abs(imgDst[i][j].g - rgb[1]);
			int d3 = abs(imgDst[i][j].b - rgb[2]);

			if(d1 > threshold || d2 > threshold || d3 > threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
}

IplImage* dimGrays(IplImage* src, int threshold, float percent)
{
	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - imgDst[i][j].g);
			int d2 = abs(imgDst[i][j].r - imgDst[i][j].b);
			int d3 = abs(imgDst[i][j].g - imgDst[i][j].b);

			if(d1 < threshold && d2 < threshold && d3 < threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
	return dst;
}

void dimGrays(IplImage* src, IplImage* dst, int threshold, float percent)
{
	dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			int d1 = abs(imgDst[i][j].r - imgDst[i][j].g);
			int d2 = abs(imgDst[i][j].r - imgDst[i][j].b);
			int d3 = abs(imgDst[i][j].g - imgDst[i][j].b);

			if(d1 < threshold && d2 < threshold && d3 < threshold) {
				imgDst[i][j].r = int(imgDst[i][j].r * percent)%256;
				imgDst[i][j].g = int(imgDst[i][j].g * percent)%256;
				imgDst[i][j].b = int(imgDst[i][j].b * percent)%256;
			}
		}
	}
}
