/*
	This code implements a method of to binarize a given document image. This method improves upon the Niblacks method
	by incorporating the contrast value of the image while the thresholding process. Specefically the noise that is generated 
	in the Niblack thresholding is removed by this method of thresholding.
	The contrast information is incorporated by the following procedure:
		-Determine the contrast value of the image.
		-Classify the pixel as a background if the contrast value if lesser than 10% of the maximum contrast, else
		 use Niblack thresholding as usual.
	A link to the paper describing this in detail will be added if the paper is accepted :) else a link to the paper will be added.
*/
#include "myBin.h"

int main(int argc,char *argv[])
{
	IplImage* inImage=cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

	/*Create the output image*/
	IplImage* outImage=cvCreateImage(cvSize(inImage->width,inImage->height),inImage->depth,inImage->nChannels);

	/*Create the ContrastArray image*/
	IplImage* ContrastArray=cvCreateImage(cvSize(inImage->width,inImage->height),inImage->depth,inImage->nChannels);

	/*Array to hold the local Niblack threshold and modified threshold values*/
	uchar *NibThresh=NULL;

	char *out_filename="-OutConNib.jpg",name_buff[25]={0};

	/*Performing standard error checks*/
	if(argc<2){
		printf("Usage:<file_name>\n");
		goto CLEAN_UP;
	}
	
	/*Generating output filename. 
	NOTE: This relies on the input image name having a file extension of 3 letters (for example- 04.bmp)*/
	strncpy(name_buff,argv[1],(strlen(argv[1])-4));
	out_filename=strncat(name_buff,out_filename,14);
	printf("main:Output filename:%s\n",out_filename);

	/*Calculating the local Niblack threshold method*/
	NibThresh=myNibThresh(inImage);

	/*Determines the contrast image of the input image*/
	myLocalContrast(inImage,ContrastArray,0);

	/*Threshold the image*/
	myThreshold(inImage,outImage,ContrastArray,NibThresh);

	/*Saving the output image*/
	if(cvSaveImage((const char*)out_filename,outImage,(const int*)0)==0){
		printf("myLocalThresh:Error while saving image\n");
		goto CLEAN_UP;
	}

	/*Release memory*/
	CLEAN_UP:
		free(NibThresh);
		cvReleaseImage(&inImage);
		cvReleaseImage(&outImage);
		cvReleaseImage(&ContrastArray);
		cvWaitKey(0);
		return;
}