

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cmath>

#include <libfeat/sift.h>
#include <libfeat/imgfeatures.h>
#include <libfeat/utils.h>

#include "../../internals/Registry.hpp"
#include "../../vision/Canvas.hpp"
#include "../../vision/Camera.hpp"
#include "../../vision/Window.hpp"


int main(int argc, char** argv)
{

	Vision::Window* windowMain = new Vision::Window("main");
	Vision::Window* windowSift = new Vision::Window("sift");
	Vision::Camera* camera = new Vision::Camera();
	
	Internals::Registry::add("windowMain", windowMain);
	Internals::Registry::add("windowSift", windowSift);
	Internals::Registry::add("camera", camera);

	camera->setResize(320, 240);

	IplImage* frame;
	IplImage* sift;

	struct feature* features;

	int intvls = SIFT_INTVLS;
	double sigma = SIFT_SIGMA;
	double contr_thr = SIFT_CONTR_THR;
	int curv_thr = SIFT_CURV_THR;
	int img_dbl = SIFT_IMG_DBL;
	int descr_width = SIFT_DESCR_WIDTH;
	int descr_hist_bins = SIFT_DESCR_HIST_BINS;

	while(1) {
		frame = camera->queryFrame();

		int n = 0;

		/*n = _sift_features(frame, &features, intvls, sigma, contr_thr, curv_thr,
		      img_dbl, descr_width, descr_hist_bins );*/

		sift_features(frame, &features);

		//draw_features( frame, features, n );
		windowMain->showImageWithHist(frame);

		cvReleaseImage(&frame);
		
		if(Vision::Window::keypress())
			break;
	}
	
	delete camera;
	delete windowMain;
	delete windowSift;

	return 0;
}
