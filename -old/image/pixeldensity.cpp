#include "pixeldensity.hpp"

bool densityGt(IplImage* src, int rgb[3], float percent, int threshold)
{
	IplImage* dst = cvCloneImage(src);
	RgbImage imgDst(dst);

	int totalPixels = dst->height * dst->width;
	int foundPixels = 0;

	for(int i = 0; i < dst->height; i++) {
		for(int j = 0; j < dst->width; j++) {
			int d1 = abs(imgDst[i][j].r - rgb[0]);
			int d2 = abs(imgDst[i][j].g - rgb[1]);
			int d3 = abs(imgDst[i][j].b - rgb[2]);

			if(d1 < threshold && d2 < threshold && d3 < threshold) {
				foundPixels++;
			}
		}
	}

	float ratio = float(foundPixels)/totalPixels;

	if (ratio >= percent) {
		return true;
	}
	return false;
}
