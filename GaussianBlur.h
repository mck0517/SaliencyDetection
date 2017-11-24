#ifndef _GAUSSIAN_BLUR_H_
#define _GAUSSIAN_BLUR_H_

#include "Param.h"

void GaussianBlurUsing3by3(const float *inImage, float *outImage, const int width, const int height)
{
	float tempImage[IMAGE_SIZE] = {0, };

	int index = 0;

	register int r, c, cc, rr;

	float kernel[3] = {1.0f, 2.0f, 1.0f};

	float kernelSum = 0.0f;

	float sum = 0.0f;

	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			kernelSum = 0.0f;
			sum = 0.0f;

			for(cc=-1; cc<=1; cc++)
			{
				if (((c + cc) >= 0) && ((c + cc) < width))
				{
					sum += inImage[r*width+(c+cc)] * kernel[1+cc];
					kernelSum += kernel[1+cc];
				}
			}

			tempImage[index] = sum/kernelSum;
			index++;
		}
	}


	index = 0;

	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			kernelSum = 0.0f;
			sum = 0.0f;

			for(rr=-1; rr<=1; rr++)
			{
				if (((r+rr) >= 0) && ((r+rr) < height))
				{
					sum += tempImage[(r+rr)*width+c]*kernel[1+rr];
					kernelSum += kernel[1+rr];
				}
			}

			outImage[index] = sum/kernelSum;
			index++;
		}
	}

}

#endif