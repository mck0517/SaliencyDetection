#ifndef _SALIENCY_MSS_H_
#define _SALIENCY_MSS_H_

#include "Data.h"
#include "GaussianBlur.h"

void CreateIntegralImage(const float *inputImage, const int width, const int height, float integralImage[IMAGE_HEIGHT][IMAGE_WIDTH])
{

	register int i, j;

	int index = 0;

	for(j=0; j<height; j++)
	{
		float sumRow = 0.0f;

		for(i=0; i<width; i++)
		{
			sumRow += inputImage[index];
			index++;

			if(0 == j)
			{
				integralImage[j][i] = sumRow;
			}

			else
			{
				integralImage[j][i] = integralImage[j-1][i]+sumRow;
			}

		}

	}

}


float GetIntegralSum(const float integralImage[IMAGE_HEIGHT][IMAGE_WIDTH], const int& x1, const int& y1, const int& x2, const int& y2)
{
	float sum = 0.0f;

	if (x1-1<0 && y1-1<0)
	{
		sum = integralImage[y2][x2];
	}

	else if (x1-1<0)
	{
		sum = integralImage[y2][x2]-integralImage[y1-1][x2];
	}

	else if (y1-1<0)
	{
		sum = integralImage[y2][x2]-integralImage[y2][x1-1];
	}

	else
	{
		sum = integralImage[y2][x2]+integralImage[y1-1][x1-1]-integralImage[y1-1][x2]-integralImage[y2][x1-1];
	}

	return sum;

}


void SaliencyDetectionUsingMSS(IplImage *inputImage, unsigned char *saliencyImage)
{
	int imageW = inputImage->width;
	int imageH = inputImage->height;

	memset(RED_MSS, 0, sizeof(RED_MSS));
	memset(GREEN_MSS, 0, sizeof(GREEN_MSS));
	memset(BLUE_MSS, 0, sizeof(BLUE_MSS));

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

			RED_MSS[index+j] = SAT_UCHAR(red);
			GREEN_MSS[index+j] = SAT_UCHAR(green);
			BLUE_MSS[index+j] = SAT_UCHAR(blue);

		}
	}

	memset(LCOLOR_MSS, 0, sizeof(LCOLOR_MSS));
	memset(ACOLOR_MSS, 0, sizeof(ACOLOR_MSS));
	memset(BCOLOR_MSS, 0, sizeof(BCOLOR_MSS));

	int totalSize = IMAGE_SIZE;

	for(j=0; j<totalSize; j++)
	{
		int sR = RED_MSS[j];
		int sG = GREEN_MSS[j];
		int sB = BLUE_MSS[j];

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

		LCOLOR_MSS[j] = 116.0f * fy - 16.0f;
		ACOLOR_MSS[j] = 500.0f * (fx - fy);
		BCOLOR_MSS[j] = 200.0f * (fy - fz);

	}

	memset(LCOLOR_SMOOTH_MSS, 0, sizeof(LCOLOR_SMOOTH_MSS));
	memset(ACOLOR_SMOOTH_MSS, 0, sizeof(ACOLOR_SMOOTH_MSS));
	memset(BCOLOR_SMOOTH_MSS, 0, sizeof(BCOLOR_SMOOTH_MSS));

	GaussianBlurUsing3by3(LCOLOR_MSS, LCOLOR_SMOOTH_MSS, imageW, imageH);
	GaussianBlurUsing3by3(ACOLOR_MSS, ACOLOR_SMOOTH_MSS, imageW, imageH);
	GaussianBlurUsing3by3(BCOLOR_MSS, BCOLOR_SMOOTH_MSS, imageW, imageH);

	memset(INTEGRAL_L_IMAGE_MSS, 0, sizeof(INTEGRAL_L_IMAGE_MSS));
	memset(INTEGRAL_A_IMAGE_MSS, 0, sizeof(INTEGRAL_A_IMAGE_MSS));
	memset(INTEGRAL_B_IMAGE_MSS, 0, sizeof(INTEGRAL_B_IMAGE_MSS));

	CreateIntegralImage(LCOLOR_MSS, imageW, imageH, INTEGRAL_L_IMAGE_MSS);
	CreateIntegralImage(ACOLOR_MSS, imageW, imageH, INTEGRAL_A_IMAGE_MSS);
	CreateIntegralImage(BCOLOR_MSS, imageW, imageH, INTEGRAL_B_IMAGE_MSS);

	memset(SALIENCY_MAP_MSS, 0, sizeof(SALIENCY_MAP_MSS));

	index = 0;

	for(int j=0; j<imageH; j++)
	{
		int yoff = FIND_MIN(j, imageH-j);
		int y1 = FIND_MAX(j-yoff, 0);
		int y2 = FIND_MIN(j+yoff, imageH-1);

		for(int k=0; k<imageW; k++)
		{
			int xoff = FIND_MIN(k, imageW-k);
			int x1 = FIND_MAX(k-xoff, 0);
			int x2 = FIND_MIN(k+xoff, imageW-1);

			float area = (x2-x1+1)*(y2-y1+1);

			float lVal = GetIntegralSum(INTEGRAL_L_IMAGE_MSS, x1, y1, x2, y2) / area;
			float aVal = GetIntegralSum(INTEGRAL_A_IMAGE_MSS, x1, y1, x2, y2) / area;
			float bVal = GetIntegralSum(INTEGRAL_B_IMAGE_MSS, x1, y1, x2, y2) / area;

			SALIENCY_MAP_MSS[index] = (lVal - LCOLOR_SMOOTH_MSS[index]) * (lVal - LCOLOR_SMOOTH_MSS[index]) + 
				                      (aVal - ACOLOR_SMOOTH_MSS[index]) * (aVal - ACOLOR_SMOOTH_MSS[index]) + 
				                      (bVal - BCOLOR_SMOOTH_MSS[index]) * (bVal - BCOLOR_SMOOTH_MSS[index]);
			index++;

		}
	}


	float maxVal = 0.0f;
	float minVal = (1 << 30); 

	for(i=0; i<totalSize; i++)
	{
		if( maxVal < SALIENCY_MAP_MSS[i] )
		{
			maxVal = SALIENCY_MAP_MSS[i];
		}

		if( minVal > SALIENCY_MAP_MSS[i] )
		{
			minVal = SALIENCY_MAP_MSS[i];
		}
	}

	float range = maxVal-minVal;

	if( 0 == range )
	{
		range = 1.0f;
	}

	for(i=0; i<totalSize; i++)
	{
		saliencyImage[i] = SAT_UCHAR(((255.0f*(SALIENCY_MAP_MSS[i]-minVal))/range)+0.5f);
	}

}

#endif