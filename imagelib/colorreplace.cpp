#include <math.h>
#include "colorreplace.hpp"

IplImage* replaceColors(IplImage* src, int colors[][7]) 
{
	int len = sizeof(colors);

	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {
			for(int k = 0; k < len; k++) {
				// 0-2 are the source RGB
				// 3-5 are the destination RGB
				// 6 is the threshold of difference. 
				int d1 = abs(imgDst[i][j].r - colors[k][0]);
				int d2 = abs(imgDst[i][j].g - colors[k][1]);
				int d3 = abs(imgDst[i][j].b - colors[k][2]);

				if(d1 < colors[k][6] && d2 < colors[k][6] && d3 < colors[k][6]) {
					imgDst[i][j].r = colors[k][3];
					imgDst[i][j].g = colors[k][4];
					imgDst[i][j].b = colors[k][5];
				}
			}
		}
	}
	return dst;
}

IplImage* replaceColors(IplImage* src, int colors[][7], int other[3])
{
	int len = sizeof(colors);

	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			bool match = false;

			for(int k = 0; k < len; k++) {
				// 0-2 are the source RGB
				// 3-5 are the destination RGB
				// 6 is the threshold of difference. 
				int d1 = abs(imgDst[i][j].r - colors[k][0]);
				int d2 = abs(imgDst[i][j].g - colors[k][1]);
				int d3 = abs(imgDst[i][j].b - colors[k][2]);

				if(d1 < colors[k][6] && d2 < colors[k][6] && d3 < colors[k][6]) {
					imgDst[i][j].r = colors[k][3];
					imgDst[i][j].g = colors[k][4];
					imgDst[i][j].b = colors[k][5];
					match = true;
					break;
				}
			}
			if(!match) {
				imgDst[i][j].r = other[0];
				imgDst[i][j].g = other[1];
				imgDst[i][j].b = other[2];
			}
		}
	}
	return dst;
}

void replaceColors(IplImage* src, IplImage* dst, int colors[][7])
{
	int len = sizeof(colors);

	dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {
			for(int k = 0; k < len; k++) {
				// 0-2 are the source RGB
				// 3-5 are the destination RGB
				// 6 is the threshold of difference. 
				int d1 = abs(imgDst[i][j].r - colors[k][0]);
				int d2 = abs(imgDst[i][j].g - colors[k][1]);
				int d3 = abs(imgDst[i][j].b - colors[k][2]);

				if(d1 < colors[k][6] && d2 < colors[k][6] && d3 < colors[k][6]) {
					imgDst[i][j].r = colors[k][3];
					imgDst[i][j].g = colors[k][4];
					imgDst[i][j].b = colors[k][5];
				}
			}
		}
	}
}

void replaceColors(IplImage* src, IplImage* dst, int colors[][7], int other[3])
{
	int len = sizeof(colors);

	dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {

			bool match = false;

			for(int k = 0; k < len; k++) {
				// 0-2 are the source RGB
				// 3-5 are the destination RGB
				// 6 is the threshold of difference. 
				int d1 = abs(imgDst[i][j].r - colors[k][0]);
				int d2 = abs(imgDst[i][j].g - colors[k][1]);
				int d3 = abs(imgDst[i][j].b - colors[k][2]);

				if(d1 < colors[k][6] && d2 < colors[k][6] && d3 < colors[k][6]) {
					imgDst[i][j].r = colors[k][3];
					imgDst[i][j].g = colors[k][4];
					imgDst[i][j].b = colors[k][5];
					match = true;
					break;
				}
			}
			if(!match) {
				imgDst[i][j].r = other[0];
				imgDst[i][j].g = other[1];
				imgDst[i][j].b = other[2];
			}
		}
	}
}
