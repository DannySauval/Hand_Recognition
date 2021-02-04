/**
 * \file figure_libre.cpp
 * \brief Programme complet de la partie libre du projet en C.
 * \author Nathan RIVIERE et Danny SAUVAL
 * \version 1.0
 * \date 28 mai 2017
 *
 * Programme permettant à l'aide d'une caméra de traduire les signes effectué avec une main.
 * Source du code utilisé pour traiter la main isolée : https://www.youtube.com/watch?v=H9diqywK6NY
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "highgui.h"
#include "cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <unistd.h>
#include <time.h>

#include <SFML/Audio.hpp>

#include "fonctions.hpp"

using namespace std;
using namespace cv;

int main(){
	char key = ' '; // On initiliase la variable qui va contenir le charactère envoyé par l'utilisateur.
	int i;

	IplImage *image; 
	IplImage *hsv_img;
	IplImage *mask;
	IplImage *tmp;

	Parametres *param = (Parametres*)malloc(sizeof(Parametres)); /// On créé notre structure paramètre.
	param->clic = 0; /// On initialise la variable qui permet de savoir si un clic a eu lieu ou non

	double **hsv = (double**) malloc(6*sizeof(double*));
	for (i = 0; i < 6; i++)
		hsv[i] = (double*) malloc(3*sizeof(double));

	// Capture vidéo
	CvCapture *webcam;

	/// On ouvre le flux vidéo
	webcam = cvCreateCameraCapture(0);

	// Vérifier si l'ouverture du flux vidéo est ok
	if (!webcam){
		printf("Ouverture du flux vidéo impossible !\n");
		return 1;
	}

	/// Définition des fenêtres
	cvNamedWindow("Control", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Flux Somme", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Flux RGB", CV_WINDOW_AUTOSIZE);

	int tolerance = 20;
	cvCreateTrackbar("test", "Control", &tolerance, 255); // Permet de régler en direct la tolérance de la binarisation

	CvScalar pixel; // Pixel où on clic
	image = cvQueryFrame(webcam);
	tmp = cvCreateImage(cvGetSize(image), image->depth, 1); // Image buffer
	
	// Variables qui contiennent les sons
	sf::Music HighFive; 
	sf::Music FourFingers;
	sf::Music ThreeFingers;
	sf::Music Metal;
	sf::Music NonReconnu;

	/// On charge les sons
	if (!HighFive.openFromFile("Audio/HighFive.ogg")) 
    	return -1;
    if (!FourFingers.openFromFile("Audio/FourFingers.ogg"))
    	return -1;
    if (!ThreeFingers.openFromFile("Audio/ThreeFingers.ogg"))
    	return -1;
    if (!Metal.openFromFile("Audio/LetsRock.ogg"))
    	return -1;
    if (!NonReconnu.openFromFile("Audio/NonReconnu.ogg"))
    	return -1;

    int cpt_clic = 0; // Permet de compter le nombre de clics 

    clock_t debut;
    int je_viens_de_parler = 0;

	while(key != 'q' && key != 'Q') {
		image = cvQueryFrame(webcam);

		cvSmooth(image,image,CV_BLUR,(20,20),0); /// On floute l'image pour minimiser les parasites
		cvFlip(image, image, 1);
		hsv_img = cvCloneImage(image);
		cvCvtColor(hsv_img, hsv_img, CV_BGR2HSV);
		mask = cvCreateImage(cvGetSize(image), image->depth, 1);

		setMouseCallback("Flux RGB", CallBackFunc, param); /// Récupération infos clic

		if (param->clic == 1){ // Si on a vient de cliquer
			if (cpt_clic < 6){
				cpt_clic++;
			}
			else{
				cpt_clic = 1;
			}
			printf("%d\n", cpt_clic);
			param->clic = 0;

			/// On met à jour les valeurs h, s, v correspondant au clic effectué
			hsv[cpt_clic-1][0] = cvGet2D(hsv_img, param->y, param->x).val[0]; 
			hsv[cpt_clic-1][1] = cvGet2D(hsv_img, param->y, param->x).val[1];
			hsv[cpt_clic-1][2] = cvGet2D(hsv_img, param->y, param->x).val[2];
		}

		mask = addition_images_binarisees(hsv, tolerance, hsv_img, mask, tmp, cpt_clic);

		// Réduction des parasites (erosions/dilatations)
		for (i = 0; i < 2; i++){
			cvErode(mask, mask, 0, 2);
			cvDilate(mask, mask, 0, 2);
			cvDilate(mask, mask, 0, 2);
			cvErode(mask, mask, 0, 2);
		}

		tmp = cvCloneImage(mask); //BIDOUILLAGE

		CvMemStorage* storage = cvCreateMemStorage(); // Zone mémoire pour stocker les contours
        CvSeq* first_contour = NULL; // Pointe vers les contours
        CvSeq* maxitem = NULL;
        // On récupère les contours de l'image que l'on stock dans first_contour
        int cn = cvFindContours(mask, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0)); // CV_RETR_EXTERNAL à voir

        
        double area, max_area = 0.0; // Variable permettant de calculer l'air max des contours extraits avec cvFindContours

        CvSeq* ptr = 0;
        if(cn > 0){ // Si on a extrait un contour            
            for(ptr = first_contour; ptr!=NULL; ptr=ptr->h_next){ // Parcourt de tous les contours
                area = cvContourArea(ptr, CV_WHOLE_SEQ, 0); // Calcul l'air d'un contour (valeur absolue)
                if(area > max_area){ // On cherche l'air maximale
                    max_area = area;
                    maxitem = ptr; // Adresse du contour
                }
            }
            if(max_area > 1000){
                CvPoint pt0;
                CvMemStorage* storage1 = cvCreateMemStorage(0);
                CvMemStorage* storage2 = cvCreateMemStorage(0);
                CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage1); // Contient les points du contour
                CvSeq* hull;
                CvSeq* defects;
                
                for(int i = 0; i < maxitem->total; i++ ){ // AJout de tous les point du contour à ptseq
                    CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, maxitem, i );
                    pt0.x = p->x;
                    pt0.y = p->y;
                    cvSeqPush( ptseq, &pt0 ); // Ajout du nouveau point à la fin de ptseq
                }
                hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0); // On récupère l'enveloppe convexe du contour
                int hullcount = hull->total;
                defects = cvConvexityDefects(ptseq, hull, storage2); // On récupère les défauts de convexité : ce sont nos doigts !
                CvConvexityDefect* defectArray;  

                for(int i = 1; i <= hullcount; i++){ // On affiche l'enveloppe convexe
                    CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
                    cvLine( image, pt0, pt, CV_RGB( 255, 0, 0 ), 1, CV_AA, 0 );
                    pt0 = pt;
                }
                for(;defects;defects = defects->h_next){
                    int nomdef = defects->total; // defect amount  
                    //outlet_float( m_nomdef, nomdef );  
                    //printf(" defect no %d \n",nomdef);
                    if(nomdef == 0)  
                    	continue;  
                    // Alloc memory for defect set.     
                    //fprintf(stderr,"malloc\n");  
                    defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);  
                    // Get defect set.  
                    //fprintf(stderr,"cvCvtSeqToArray\n");  
                    cvCvtSeqToArray(defects,defectArray, CV_WHOLE_SEQ);      
                    // Draw marks for all defects.
                    int con = 0;
                    for(int i=0; i<nomdef; i++)  {
                        if(defectArray[i].depth > 80){
                            con = con + 1;
                            //printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
                            cvLine(image, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(255,255,0),1, CV_AA, 0 );  
                            cvCircle( image, *(defectArray[i].depth_point), 5, CV_RGB(0,0,255), 2, 8,0);  
                            cvCircle( image, *(defectArray[i].start), 5, CV_RGB(0,255,0), 2, 8,0);  
                            cvLine(image, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(0,255,255),1, CV_AA, 0 );  
                            cvDrawContours(image,defects,CV_RGB(0,0,0),CV_RGB(255,0,0),-1,CV_FILLED,8);
                        }
                    }

                    char txt[40] = ""; // Contient le texte qui va s'afficher à l'écran
                    char txt2[2] = "";
                    sprintf(txt2, "%d", cpt_clic);
                    if(con == 1 && cpt_clic == 6){
                        char txt1[] = "MEEEETTTTAAAAAALLLL !!!!!";
                        strcat(txt, txt1);
                        if (je_viens_de_parler == 0){
                        	je_viens_de_parler = 1;
                        	Metal.play();
                        	debut = clock();
                        }
                        if (clock()-debut >= 550000) je_viens_de_parler = 0; // On attend avant de reparler
                    }
                    else if(con == 2 && cpt_clic == 6){
                        char txt1[] = "3 doigts";
                        strcat(txt, txt1);
                        if (je_viens_de_parler == 0){
                        	je_viens_de_parler = 1;
                        	ThreeFingers.play();
                        	debut = clock();
                        }
                        if (clock()-debut >= 550000) je_viens_de_parler = 0;
                    }
                    else if(con == 3 && cpt_clic == 6){
                        char txt1[] = "4 doigts";
                        strcat(txt, txt1);
                        if (je_viens_de_parler == 0){
                        	je_viens_de_parler = 1;
                        	FourFingers.play();
                        	debut = clock();
                        }
                        if (clock()-debut >= 550000) je_viens_de_parler = 0;
                    }
                    else if(con == 4 && cpt_clic == 6){
                        char txt1[] = "High Five !";
                        strcat(txt, txt1);
                        if (je_viens_de_parler == 0){
                        	je_viens_de_parler = 1;
                        	HighFive.play();
                        	debut = clock();
                        }
                        if (clock()-debut >= 550000) je_viens_de_parler = 0;
                    }
                    else if (cpt_clic == 6){
                        char txt1[] = "Autre signe";
                        strcat(txt,txt1);
                        if (je_viens_de_parler == 0){
                        	je_viens_de_parler = 1;
                        	NonReconnu.play();
                        	debut = clock();
                        }
                        if (clock()-debut >= 550000) je_viens_de_parler = 0;
                    }
                    CvFont font;
                    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 5, CV_AA);
                    cvPutText(image, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));
                    cvPutText(image, txt2, cvPoint(image->width-100, image->height-20), &font, cvScalar(0, 0, 255, 0));
       
                    free(defectArray);  
                } 
                cvReleaseMemStorage( &storage1 );
                cvReleaseMemStorage( &storage2 );
            }
        }
        cvReleaseMemStorage( &storage );

		cvMoveWindow("Flux Somme", 0, 300);
		cvMoveWindow("Flux RGB", 1000, 300);
		cvShowImage( "Flux Somme", tmp);
		cvShowImage( "Flux RGB", image);

		cvZero(tmp);
		cvReleaseMemStorage( &storage );

		// On attend 10ms
		key = cvWaitKey(100);
	}

	cvReleaseCapture(&webcam);
	cvDestroyWindow("Flux RGB");
	cvDestroyWindow("Flux Somme");
	free(param);

	return 0;

}