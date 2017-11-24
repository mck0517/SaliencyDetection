* Various Saliency Detection Implementation using C language 
- reImplementation below papers
  1. "SALIENCY DETECTION USING MAXIMUM SYMMETRIC SURROUND", ICIP 2010(https://infoscience.epfl.ch/record/151989/files/ICIP2010.pdf)  
  2. "Frequency-tuned Salient Region Detection", CVPR 2009(https://infoscience.epfl.ch/record/135217/files/1708.pdf)  

* File Description
- Data.h:  
- Vision_Test.cpp: main fuction
- GaussianBlur.h: 3 by 3 separable gaussian kernel fuction
- Param.h: 
- SaliencyFT.h:
- SaliencyMSS.h:  
- Util.h: image I/O handling for OpenCV.

* Program Description
- You can use any kind of OpenCV Version.
- For the minimization of dynamic memory allocation, I sometimes use array buffer. Thus, You must set the width and height(IMAGE_WIDTH, IMAGE_HEIGHT in Param.h) of the image in advance. 
- OpenCV was used only for image reading fuction(ex: cvLoadImage("mandrill.bmp", CV_LOAD_IMAGE_COLOR)).


 
 

