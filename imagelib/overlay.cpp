#include "overlay.hpp"

IplImage* overlayEdges(IplImage* edges, IplImage* other)
{
	IplImage* dst = cvCloneImage(other);

	BwImage  imgEdge(edges);
	RgbImage imgDst(dst);

	for (int i = 0; i < edges->height; i++) {
		for (int j = 0; j < edges->width; j++) {
			if (imgEdge[i][j] == 255) {
				imgDst[i][j].r = 255;
				imgDst[i][j].g = 255;
				imgDst[i][j].b = 255;
			}
		}
	}

	return dst;
}
