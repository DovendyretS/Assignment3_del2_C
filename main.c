#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <stdio.h>
#include <stdlib.h>

IplImage* src;
IplImage* src_gray;
FILE *fp;
FILE *fpImage;
int *intensenties;
int buffer;

int main() {

    // command used for compiling .c file
    // gcc main.c -o main -lopencv_core -lopencv_highgui -lopencv_imgproc

    intensenties = (int *)calloc(256, sizeof(int));

    //load image taken from RPi camera
    src = cvLoadImage("/home/pi/billed1.jpg", CV_LOAD_IMAGE_COLOR);
    if (src == NULL)
    {
        printf("Error no image found\n");
        return 0;
    }

    //create image
    src_gray = cvCreateImage(cvGetSize(src), 8, 1);

    //convert image to gray_scale
    cvCvtColor(src, src_gray, CV_BGR2GRAY);

    //save gray_scale image
    cvSaveImage("grey_scale.jpg", src_gray,0);

    fpImage = fopen("grey_scale.jpg", "r");

    //read intensity values of all pixels
    for (int i = 0; i < src_gray->width*src_gray->height; ++i)
    {
        fread(&buffer, 1, 1, fpImage);
        *(intensenties + buffer) += 1;
    }
    fclose(fpImage);

    //write data to HISTOGRAM.SOL
    fp = fopen("HISTOGRAM.SOL","w");

    int zeroIntensity = 0;
    for (int h = 0; h < 256 ; ++h)
    {
        if (*(intensenties + h) == 0)
            zeroIntensity++;
    }

    //number for intensity pictures calculated
    fprintf(fp, "%d\n", 256-zeroIntensity);

    //resolution calculated
    fprintf(fp, "%d%s%d\n", src_gray->height,"x",src_gray->width);

    //number of pixels in each different intensity
    for (int j = 0; j < 256 ; ++j)
    {
        fprintf(fp,"%d %d \n ", *(intensenties + j),j);
    }

    fclose(fp);
    cvReleaseImage(&src_gray);
    cvReleaseImage(&src);

}