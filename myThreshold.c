/*
	This function performs the actual thresholding operations based on the input image graylevel and 
	the threshold values determined elsewhere.
	Here:	inImage is the input grayscale image.
			outImage is the output image.
			ThreshVec is the threshold values determined beforehand.
*/
#include "myBin.h"
void myThreshold(IplImage* inImage, IplImage* outImage, IplImage* ContrastArray,uchar* ThreshVec)
{
	uchar *imagedata=NULL,*outimdata=NULL,*conimdata=NULL;
	int height,width,step,channels,maxCon=0,temp=0;
	int x,y,k;

	/*Obtain input image parameters for use to traverse image*/
	height = inImage->height;	/*Image height in pixels*/
	width = inImage->width;		/*Image width in pixels*/
	step = inImage->widthStep;	/*The size of an aligned image row, in bytes*/
	channels = inImage->nChannels;
	imagedata = (uchar *)inImage->imageData;

	/*Thresholding the image based on the threshold values calculated.*/
	/*Copy input image to the output image*/
	cvSetZero(outImage);
	cvCopy(inImage,outImage,NULL);
	outimdata = (uchar *)outImage->imageData;
	conimdata= (uchar*)ContrastArray->imageData;

	/*Find the largest value of the contrast for use in the thresholding*/
	temp=conimdata[0];
	for(y=0;y<height;y++)		
		for(x=0;x<width;x++)	
			for(k=0;k<channels;k++){
				if(conimdata[y*step+x*channels+k]>temp)
					temp=conimdata[y*step+x*channels+k];
			}
	maxCon=temp;

	/*Perform Niblack threshold only if the contrast value is greater than 10% of the maximum contrast value*/
	for(y=0;y<height;y++)		
		for(x=0;x<width;x++)	
			for(k=0;k<channels;k++){
				if(conimdata[y*step+x*channels+k]>(0.1*maxCon)){
					if(imagedata[y*step+x*channels+k]>ThreshVec[y*step+x*channels+k])
						outimdata[y*step+x*channels+k]=WHITE;
					else
						outimdata[y*step+x*channels+k]=BLACK;
				}
				else {
					outimdata[y*step+x*channels+k]=WHITE;	
				}
			}
	
	printf("myThreshold:myThreshold exited\n");
	return;
}
