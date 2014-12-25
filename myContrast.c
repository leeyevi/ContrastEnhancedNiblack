/*
	This function is called by myLocalContrast and operates only on the data in the image ROI
	and finds the contrast as given by the expression:
						Contrast=(max_value-centralPixel)/(max_value)
	NOTE:	The expression yields a normalized contrast feature i.e. it is lesser than one. Because of that 
			in an expression that uses the contrast value returned by this function it is necessary to multiply
			the contrast by the maximum intensity allowed (here, 255).
	Here:	roi is the ROI over which the threshold is estimated.
			centralPixel is the central pixel of the ROI. (I pass this only since its easier than finding the central pixel 
			within this function.)
*/
#include "myBin.h"
float myContrast(IplImage* roi,uchar centralPixel)
{
	uchar max_value=0,temp=0;
	float contrast=0;
	int height,width,step,channels;
	uchar *imagedata,easy_array[CONMASK_SIZE*CONMASK_SIZE];
	int i,j,k; 

	/*Obtain the roi parameters*/
	height = roi->height;
	width = roi->width;		/*Image width in pixels*/
	step = roi->widthStep;	/*The size of an aligned image row, in bytes*/
	channels =roi->nChannels;
	imagedata = (uchar *)roi->imageData;

	/*Copying the image pixel values into a simpler local array.The elements
	  here are stored in a similar way to that of imageData in IplImage except 
	  that the width itself is used in place of the widthStep*/
	for(i=0;i<height;i++)		
		for(j=0;j<width;j++)	
			for(k=0;k<channels;k++)
				easy_array[i*width+j+k]=imagedata[i*step+j*channels+k];

	/*Finding the maximum value of the intensity within the ROI*/
	temp=easy_array[0];
	for(i=0;i<CONMASK_SIZE*CONMASK_SIZE;i++){
		if(easy_array[i]>temp)
			temp=easy_array[i];
	}
	max_value=temp;

	/*Estimate the contrast value using the said expression*/
	if(max_value==0)
		return 0;
	else{
		contrast=((float)(abs(max_value-centralPixel)))/((float)(max_value));
		return contrast; 
	}
}