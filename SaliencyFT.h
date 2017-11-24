#ifndef _SALIENCY_FT_H_
#define _SALIENCY_FT_H_

#include "Data.h"
#include "GaussianBlur.h"

void SaliencyDetectionUsingFT(IplImage *inputImage, unsigned char *saliencyImage)
{
	int imageW = inputImage->width;
	int imageH = inputImage->height;

	memset(RED_FT, 0, sizeof(RED_FT));
	memset(GREEN_FT, 0, sizeof(GREEN_FT));
	memset(BLUE_FT, 0, sizeof(BLUE_FT));

	register int i, j;

	int index = 0;

	for(i=0; i<imageH; i++)
	{
		index = i*imageW;

		for(j=0; j<imageW; j++)
		{
			unsigned char red = (unsigned char)inputImage->imageData[(i*inputImage->widthStep)+j*3+2]; 
			unsigned char green = (unsigned char)inputImage->imageData[(i*inputImage->widthStep)+j*3+1]; 
			unsigned char blue = (unsigned char)inputImage->imageData[(i*inputImage->widthStep)+j*3+0];

			RED_FT[index+j] = SAT_UCHAR(red);
			GREEN_FT[index+j] = SAT_UCHAR(green);
			BLUE_FT[index+j] = SAT_UCHAR(blue);

		}
	}

	memset(LCOLOR_FT, 0, sizeof(LCOLOR_FT));
	memset(ACOLOR_FT, 0, sizeof(ACOLOR_FT));
	memset(BCOLOR_FT, 0, sizeof(BCOLOR_FT));

	int totalSize = IMAGE_SIZE;

	for(j=0; j<totalSize; j++)
	{
		int sR = RED_FT[j];
		int sG = GREEN_FT[j];
		int sB = BLUE_FT[j];

		//sRGB to XYZ conversion
		float R = sR/255.0f;
		float G = sG/255.0f;
		float B = sB/255.0f;

		float r, g, b;

		if (R <= 0.04f)
		{
			r = R / 12.92f;
		}
		else
		{
			r = pow((R + 0.05f) / 1.05f, 2.4f);
		}


		if (G <= 0.04f)
		{
			g = G / 12.92f;
		}
		else
		{
			g = pow((G + 0.05f) / 1.05f, 2.4f);
		}

		if (B <= 0.04f)
		{
			b = B / 12.92f;
		}

		else
		{
			b = pow((B + 0.05f) / 1.05f, 2.4f);
		}

		float X = r * 0.41f + g * 0.35f + b * 0.18f;
		float Y = r * 0.21f + g * 0.71f + b * 0.07f;
		float Z = r * 0.01f + g * 0.11f + b * 0.95f;

		// XYZ to LAB conversion
		float epsilon = 0.008f; 
		float kappa = 903.3f; 

		float Xr = 0.95f; 
		float Yr = 1.0f; 
		float Zr = 1.08f; 
		float factor = 0.33f;

		float xr = X / Xr;
		float yr = Y / Yr;
		float zr = Z / Zr;

		float fx, fy, fz;

		if (xr > epsilon)
		{
			fx = pow(xr, factor);
		}
		else
		{
			fx = (kappa * xr + 16.0f) / 116.0f;
		}

		if (yr > epsilon)
		{
			fy = pow(yr, factor);
		}
		else
		{
			fy = (kappa * yr + 16.0f) / 116.0f;
		}

		if (zr > epsilon)
		{
			fz = pow(zr, factor);
		}
		else
		{
			fz = (kappa * zr + 16.0f) / 116.0f;
		}

		LCOLOR_FT[j] = 116.0f * fy - 16.0f;
		ACOLOR_FT[j] = 500.0f * (fx - fy);
		BCOLOR_FT[j] = 200.0f * (fy - fz);

	}

	float sumL = 0.0f, sumA = 0.0f, sumB = 0.0f;
	float avgL = 0.0f, avgA = 0.0f, avgB = 0.0f;

	for(i=0; i<totalSize; i++)
	{
		sumL += LCOLOR_FT[i];
		sumA += ACOLOR_FT[i];
		sumB += BCOLOR_FT[i];
	}

	avgL = sumL/(float)totalSize;
	avgA = sumA/(float)totalSize;
	avgB = sumB/(float)totalSize;

	memset(SALIENCY_MAP_FT, 0, sizeof(SALIENCY_MAP_FT));

	memset(LCOLOR_SMOOTH_FT, 0, sizeof(LCOLOR_SMOOTH_FT));
	memset(ACOLOR_SMOOTH_FT, 0, sizeof(ACOLOR_SMOOTH_FT));
	memset(BCOLOR_SMOOTH_FT, 0, sizeof(BCOLOR_SMOOTH_FT));

	GaussianBlurUsing3by3(LCOLOR_FT, LCOLOR_SMOOTH_FT, imageW, imageH);
	GaussianBlurUsing3by3(ACOLOR_FT, ACOLOR_SMOOTH_FT, imageW, imageH);
	GaussianBlurUsing3by3(BCOLOR_FT, BCOLOR_SMOOTH_FT,  imageW, imageH);

	for(i=0; i<totalSize; i++)
	{
		SALIENCY_MAP_FT[i] = (LCOLOR_SMOOTH_FT[i]-avgL)*(LCOLOR_SMOOTH_FT[i]-avgL) + 
							 (ACOLOR_SMOOTH_FT[i]-avgA)*(ACOLOR_SMOOTH_FT[i]-avgA) + 
							 (BCOLOR_SMOOTH_FT[i]-avgB)*(BCOLOR_SMOOTH_FT[i]-avgB);
	}

	float maxVal = 0.0f;
	float minVal = (1 << 30); //float minVal = 100000.0f; 

	for(i=0; i<totalSize; i++)
	{
		if( maxVal < SALIENCY_MAP_FT[i] )
		{
			maxVal = SALIENCY_MAP_FT[i];
		}

		if( minVal > SALIENCY_MAP_FT[i] )
		{
			minVal = SALIENCY_MAP_FT[i];
		}
	}

	float range = maxVal-minVal;

	if( 0 == range )
	{
		range = 1.0f;
	}

	for(i=0; i<totalSize; i++)
	{
		saliencyImage[i] = SAT_UCHAR(((255.0f*(SALIENCY_MAP_FT[i]-minVal))/range)+0.5f);
	}

}

#endif