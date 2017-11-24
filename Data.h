#ifndef _DATA_H_
#define _DATA_H_

//1. MAXIMUM SYMMETRIC SURROUND SALIENCY DETECTION
//RGB Color 
static unsigned char RED_MSS[IMAGE_SIZE] = {0, };
static unsigned char GREEN_MSS[IMAGE_SIZE] = {0, };
static unsigned char BLUE_MSS[IMAGE_SIZE] = {0, };

//LAB Color 
static float LCOLOR_MSS[IMAGE_SIZE] = {0, };
static float ACOLOR_MSS[IMAGE_SIZE] = {0, };
static float BCOLOR_MSS[IMAGE_SIZE] = {0, };

//Smoothed LAB Color 
static float LCOLOR_SMOOTH_MSS[IMAGE_SIZE] = {0, };
static float ACOLOR_SMOOTH_MSS[IMAGE_SIZE] = {0, };
static float BCOLOR_SMOOTH_MSS[IMAGE_SIZE] = {0, };

//LAB Color Integral Image  
static float INTEGRAL_L_IMAGE_MSS[IMAGE_HEIGHT][IMAGE_WIDTH] = {0, };
static float INTEGRAL_A_IMAGE_MSS[IMAGE_HEIGHT][IMAGE_WIDTH] = {0, };
static float INTEGRAL_B_IMAGE_MSS[IMAGE_HEIGHT][IMAGE_WIDTH] = {0, };

//Result Saliency Image
static float SALIENCY_MAP_MSS[IMAGE_SIZE] = {0, };


//2. FREQUENCY TUNDED SALIENCY DETECTION
//RGB Color 
static unsigned char RED_FT[IMAGE_SIZE] = {0, };
static unsigned char GREEN_FT[IMAGE_SIZE] = {0, };
static unsigned char BLUE_FT[IMAGE_SIZE] = {0, };

//LAB Color 
static float LCOLOR_FT[IMAGE_SIZE] = {0, };
static float ACOLOR_FT[IMAGE_SIZE] = {0, };
static float BCOLOR_FT[IMAGE_SIZE] = {0, };

//Smoothed LAB Color 
static float LCOLOR_SMOOTH_FT[IMAGE_SIZE] = {0, };
static float ACOLOR_SMOOTH_FT[IMAGE_SIZE] = {0, };
static float BCOLOR_SMOOTH_FT[IMAGE_SIZE] = {0, };

//Result Saliency Image
static float SALIENCY_MAP_FT[IMAGE_SIZE] = {0, };

#endif