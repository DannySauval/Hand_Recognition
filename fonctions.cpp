#include <stdio.h>
#include <stdlib.h>
#include "highgui.h"
#include "cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <unistd.h>

#include "fonctions.hpp"

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
     if  (event == EVENT_LBUTTONDOWN){
          Parametres *p = (Parametres*) userdata;
          p->clic = 1;
          p->x = x;
          p->y = y;
     }
}

IplImage *addition_images_binarisees(double **hsv, int tolerance, IplImage *image, IplImage *mask, IplImage *tmp, int cpt_clic){
	int i;
	switch (cpt_clic){
		case 1:
			cvInRangeS(image, cvScalar(hsv[0][0]-tolerance, hsv[0][1]-tolerance, hsv[0][2]-tolerance, 255), cvScalar(hsv[0][0]+tolerance, hsv[0][1]+tolerance, hsv[0][2]+tolerance,255), mask);
			break;
		case 2:
			for (i = 0; i < 2; i++){
				cvInRangeS(image, cvScalar(hsv[i][0]-tolerance, hsv[i][1]-tolerance, hsv[i][2]-tolerance, 255), cvScalar(hsv[i][0]+tolerance, hsv[i][1]+tolerance, hsv[i][2]+tolerance,255), mask);
				cvAdd(mask, tmp, tmp, NULL);
			}
			mask = tmp;
			break;
		case 3:
			for (i = 0; i < 3; i++){
				cvInRangeS(image, cvScalar(hsv[i][0]-tolerance, hsv[i][1]-tolerance, hsv[i][2]-tolerance, 255), cvScalar(hsv[i][0]+tolerance, hsv[i][1]+tolerance, hsv[i][2]+tolerance,255), mask);
				cvAdd(mask, tmp, tmp, NULL);
			}
			mask = tmp;
			break;
		case 4:
			for (i = 0; i < 4; i++){
				cvInRangeS(image, cvScalar(hsv[i][0]-tolerance, hsv[i][1]-tolerance, hsv[i][2]-tolerance, 255), cvScalar(hsv[i][0]+tolerance, hsv[i][1]+tolerance, hsv[i][2]+tolerance,255), mask);
				cvAdd(mask, tmp, tmp, NULL);
			}
			mask = tmp;
			break;
		case 5:
			for (i = 0; i < 5; i++){
				cvInRangeS(image, cvScalar(hsv[i][0]-tolerance, hsv[i][1]-tolerance, hsv[i][2]-tolerance, 255), cvScalar(hsv[i][0]+tolerance, hsv[i][1]+tolerance, hsv[i][2]+tolerance,255), mask);
				cvAdd(mask, tmp, tmp, NULL);
			}
			mask = tmp;
			break;
		case 6:
			for (i = 0; i < 6; i++){
				cvInRangeS(image, cvScalar(hsv[i][0]-tolerance, hsv[i][1]-tolerance, hsv[i][2]-tolerance, 255), cvScalar(hsv[i][0]+tolerance, hsv[i][1]+tolerance, hsv[i][2]+tolerance,255), mask);
				cvAdd(mask, tmp, tmp, NULL);
			}
			mask = tmp;
			break;
	}

	return mask;
}