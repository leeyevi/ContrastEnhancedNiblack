/*
	This function populates the contrast feature array; which too is represented as a image 
	(Though maybe the best data structure to use is the cvArr or cvMat data structure)
	Here:	The inImage is the original image. 
			The ContrastArray is the empty array that is populated by this function.
			SAVE is a parameter that indicates if the function should save the resultant contrast image as well.
*/
#include "myBin.h"
void myLocalContrast(IplImage* inImage, IplImage* ContrastArray, int SAVE)
{
	/*Create the padded image.*/
	IplImage* paddedImage=cvCreateImage(cvSize(inImage->width+CONMASK_SIZE-1,inImage->height+CONMASK_SIZE-1),inImage->depth,inImage->nChannels);
	
	/*Creating the image to hold the subimage i.e. the ROI*/
	IplImage *roi=cvCreateImage(cvSize(CONMASK_SIZE,CONMASK_SIZE),inImage->depth,inImage->nChannels); 
	
	/*Variable definitions & declarations*/
	uchar *conImagedata=NULL,*imagedata=NULL, locContrast=0;
	char *filename="OutContrast.bmp";
	int height,width,step,channels;
	int x,y,k;

	if((SAVE!=0) && (SAVE!=1)){
		printf("myLocalThresh:Incorrect parameter passed\n");
		goto CLEAN_UP;
	}

	/*Creating the padded image. The paddedImage is of use in functions which run the sliding window on the image*/
	cvCopyMakeBorder(inImage,paddedImage,cvPoint(CONMASK_SIZE/2,CONMASK_SIZE/2),IPL_BORDER_CONSTANT,cvScalarAll(BLACK));

	/*Obtain input image parameters for use to traverse and obtain features for every pixel*/
	height = inImage->height;	/*Image height in pixels*/
	width = inImage->width;		/*Image width in pixels*/
	step = inImage->widthStep;	/*The size of an aligned image row, in bytes*/
	channels = inImage->nChannels;
	conImagedata = (uchar*)ContrastArray->imageData;
	imagedata=(uchar*)inImage->imageData;

	/*Goes over every pixel and calls the myContrast function that finds the value of Con(x,y) as defined in
	  the paper.*/
	for(y=0;y<height;y++)		
		for(x=0;x<width;x++)
			for(k=0;k<channels;k++){
				cvSetImageROI(paddedImage,cvRect(x,y,CONMASK_SIZE,CONMASK_SIZE));
				cvSetZero(roi);
				cvCopy(paddedImage,roi,NULL);
				locContrast=myContrast(roi,imagedata[y*step+x*channels+k])*255; /*Look at myContrast comment to understand multiplication*/
				conImagedata[y*step+x*channels+k]=locContrast;
				cvResetImageROI(paddedImage);
			}

	/*Save the contrast image to disk*/
	if(SAVE==1){
		if(cvSaveImage((const char*)filename,ContrastArray,(const int*)0)==0){
			printf("myLocalContrast:Error while saving image\n");
			goto ERROR_CLEAN_UP;
		}
		printf("myLocalContrast:OutConrast.bmp saved to disk\n");
	}

	/*Error free clean up*/
	CLEAN_UP:
		cvReleaseImage(&paddedImage);
		cvReleaseImage(&roi);
		printf("myLocalContrast:myLocalContrast exited successfully\n");
		return;


	/*Clean up code in the case of an error (in this case both error free and error clean ups are alike)*/
	ERROR_CLEAN_UP:
		cvReleaseImage(&paddedImage);
		cvReleaseImage(&roi);
		printf("myLocalContrast:myLocalContrast exited in error\n");
		return;
}