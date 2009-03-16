#include "sepchannel.hpp"

IplImage* sepChannel(IplImage* src, char channel)
{
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);

	RgbImage imgSrc(src);
	BwImage  imgDst(dst);

	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			switch(int(channel)) {
				case 'r':
				case 'R':
					imgDst[i][j] = imgSrc[i][j].r;
					break;
				case 'g':
				case 'G':
					imgDst[i][j] = imgSrc[i][j].g;
					break;
				case 'b':
				case 'B':
					imgDst[i][j] = imgSrc[i][j].b;
					break;
			}
		}
	}

	return dst;
}

void sepChannel(IplImage* src, IplImage* dst, char channel)
{
	RgbImage imgSrc(src);
	BwImage  imgDst(dst);

	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			switch(int(channel)) {
				case 'r':
				case 'R':
					imgDst[i][j] = imgSrc[i][j].r;
					break;
				case 'g':
				case 'G':
					imgDst[i][j] = imgSrc[i][j].g;
					break;
				case 'b':
				case 'B':
					imgDst[i][j] = imgSrc[i][j].b;
					break;
			}
		}
	}
}


IplImage* sepChannel(IplImage* src, float channels[3])
{
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);

	RgbImage imgSrc(src);
	RgbImage imgDst(dst);

	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			imgDst[i][j].r = int(imgSrc[i][j].r * channels[0])%256;
			imgDst[i][j].g = int(imgSrc[i][j].g * channels[1])%256;
			imgDst[i][j].b = int(imgSrc[i][j].b * channels[2])%256;
		}
	}

	return dst;
}

void sepChannel(IplImage* src, IplImage* dst, float channels[3])
{
	RgbImage imgSrc(src);
	RgbImage imgDst(dst);

	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			imgDst[i][j].r = int(imgSrc[i][j].r * channels[0])%256;
			imgDst[i][j].g = int(imgSrc[i][j].g * channels[1])%256;
			imgDst[i][j].b = int(imgSrc[i][j].b * channels[2])%256;
		}
	}
}
