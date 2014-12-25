/*
	This is called by myLocalThresh. This segment only implements the expression that estimates 
	the Niblack threshold. Given by:
					threshold=mean-(constant*standard deviation)
	Here:	roi is the ROI of size MASK_SIZE x MASK_SIZE over which the Niblack threshold is found.
			The function returns the Niblack threshold for a given pixel at the center of the ROI.
*/

#include "myBin.h"
int myNiblack(IplImage *roi)
{
	int NiblackThreshold;
	const double kval=0.5;
	CvScalar loc_mean,loc_SD;
	loc_mean=cvAvg(roi,NULL);
	cvAvgSdv(roi,&loc_mean,&loc_SD,NULL);
	/*Finding the NiblackThreshold using the formula:
	  mean_loc-(loc_SD*kval)
	  We use val[0] since we are dealing with a single channel image
	  For using multiple channel images the reference to the element 
	  in the struct mean_loc or mean_SD should be replaced by a reference 
	  to the whole struct*/
	NiblackThreshold=(int)(loc_mean.val[0]-(loc_SD.val[0]*kval));
	return NiblackThreshold;
}
