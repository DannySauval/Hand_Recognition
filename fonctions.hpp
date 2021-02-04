#ifndef _FONCTIONS_
#define _FONCTIONS_

typedef struct Parametres Parametres;
struct Parametres{
	int x;
	int y;
	int clic;
};

void CallBackFunc(int event, int x, int y, int flags, void* userdata);
IplImage *addition_images_binarisees(double **hsv, int tolerance, IplImage *image, IplImage *mask, IplImage *tmp, int cpt_clic);
double **hsv_update(double **hsv, IplImage *hsv_img, Parametres *param, int cpt_clic);

#endif
