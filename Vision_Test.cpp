// Vision_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Param.h"
#include "Util.h"
#include "SaliencyMSS.h"
#include "SaliencyFT.h"

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *iplInputImage = cvLoadImage("mandrill.bmp", CV_LOAD_IMAGE_COLOR);

	const int imageWidth = iplInputImage->width;
	const int imageHeight = iplInputImage->height;

	assert(imageWidth == IMAGE_WIDTH || imageHeight == IMAGE_HEIGHT || iplInputImage->nChannels == 3);

	unsigned char *outputMSSimage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
	IplImage *iplOutputMSSimage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);

	unsigned char *outputFTimage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
	IplImage *iplOutputFTimage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);

	//////////////////////////////////////////////////////////////////////////
	//Run Algorithm
	//Case 1: MAXIMUM SYMMETRIC SURROUND
	SaliencyDetectionUsingMSS(iplInputImage, outputMSSimage);

	//Case 2: Frequency-tuned
	SaliencyDetectionUsingFT(iplInputImage, outputFTimage);

	//Save result image
	ConvertBufferToIplImage(outputMSSimage, iplOutputMSSimage, imageWidth, imageHeight);
	cvSaveImage("SaliencyMSS.bmp", iplOutputMSSimage);

	ConvertBufferToIplImage(outputFTimage, iplOutputFTimage, imageWidth, imageHeight);
	cvSaveImage("SaliencyFT.bmp", iplOutputFTimage);
	//////////////////////////////////////////////////////////////////////////
	
	cvReleaseImage(&iplOutputMSSimage);
	free(outputMSSimage);

	cvReleaseImage(&iplOutputFTimage);
	free(outputFTimage);

	printf("\n");
	printf("Press enter for exit!\n");

	getchar();

	return 0;
}

