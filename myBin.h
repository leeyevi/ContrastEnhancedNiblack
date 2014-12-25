#include <stdio.h>
#include <math.h>
#include <string.h>
#include <highgui.h>
#include <cv.h>


#define MASK_SIZE 25	/*Mask size to define the local neighbourhood over which thresholds are found*/
#define CONMASK_SIZE 10	/*Specifies the size of the mask used for the estimation of the local contrasts*/

#define WHITE 255	/*For clarity, specifying the intensity value that corresponds to the white intensity level*/
#define BLACK 0		/*For clarity, specifying the intensity value that corresponds to the black intensity level*/

/*Finds the local thresholds and thresholds the input image*/
extern uchar* myNibThresh(IplImage* inImage);
/*Called only by myNibThresh to find the local threshold value in the subimage*/
extern int myNiblack(IplImage *inImage);

/*Finds the contrast image*/
extern void myLocalContrast(IplImage* inImage, IplImage* ContrastArray, int SAVE);
/*Called only by myLocalContrast to find contrast values for the subimage*/
extern float myContrast(IplImage* roi,uchar centralPixel);

/*Performs the thresholding operation*/
extern void myThreshold(IplImage* inImage, IplImage* outImage, IplImage* ContrastArray,uchar* ThreshVec);