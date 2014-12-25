/* 
	This function finds the adapted threshold at every pixel. 
	NOTE:	Since this threshold is used in conjunction with the contrast value one thing that can be done to 
			save on computation here is that the contrast values can be passed in addition to the input image 
			and the threshold can be estimated only for pixels that satisfy the said contrast criterion.
	Here:	inImage is the unpadded original image.
			Return value for the function is the array that contains the threshold values for every pixel.
*/

#include "myBin.h"
uchar* myNibThresh(IplImage* inImage)
{
	/*Creating the image to hold the subimage i.e. the ROI*/
	IplImage *roi=cvCreateImage(cvSize(MASK_SIZE,MASK_SIZE),inImage->depth,inImage->nChannels); 

	/*Creating the padded image. This will be of use in the functions that run a sliding window on the image*/
	IplImage *paddedImage=cvCreateImage(cvSize(inImage->width+MASK_SIZE-1,inImage->height+MASK_SIZE-1),inImage->depth,inImage->nChannels);

	/*The ImageSize for obtaining the array to hold the threshold values for each pixel*/
	int ImageSize=inImage->imageSize;
	
	/*The array to hold the local threshold values of size=ImageSize.*/
	uchar *NibThresh=(uchar*)malloc(sizeof(uchar)*ImageSize);
	
	uchar *imagedata=NULL;
	int height,width,step,channels;
	int x,y,k;
	int LocalThresh=0;

	/*Error checks*/
	if(NibThresh==NULL){ 
		printf("myLocalThresh:Call to malloc for feature vectors unsuccessful\n");
		goto CLEAN_UP;
	}

	/*Creating the padded image over which the sliding window will be run when local thresholds are calculated*/
	cvCopyMakeBorder(inImage,paddedImage,cvPoint(MASK_SIZE/2,MASK_SIZE/2),IPL_BORDER_CONSTANT,cvScalarAll(BLACK));

	/*Obtain input image parameters for use to traverse and obtain threshold for every pixel*/
	height = inImage->height;	/*Image height in pixels*/
	width = inImage->width;		/*Image width in pixels*/
	step = inImage->widthStep;	/*The size of an aligned image row, in bytes*/
	channels = inImage->nChannels;
	imagedata = (uchar *)inImage->imageData;

	/*The following code iterates over every target pixel p, in the image and calls the 
	  appropriate function to find the local threshold.*/
	for(y=0;y<height;y++)		
		for(x=0;x<width;x++)				
			for(k=0;k<channels;k++){
						cvSetImageROI(paddedImage,cvRect(x,y,MASK_SIZE,MASK_SIZE));	
						cvSetZero(roi);								/*Set ROI image data to zero*/
						cvCopy(paddedImage,roi,NULL);				/*Copy relevant image data to roi*/
						NibThresh[y*step+x*channels+k]=myNiblack(roi);
						cvResetImageROI(paddedImage);
					}

	/*Free acquired memory*/
	CLEAN_UP:
 			cvReleaseImage(&paddedImage);
 			cvReleaseImage(&roi);
 			printf("myNibThresh:myNibThresh exited\n");
 			return NibThresh;
}