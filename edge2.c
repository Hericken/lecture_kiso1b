#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "import.h"
#include "export.h"
#include "imgutil.h"

void edge()
{
    int i;
    FILE *fpIn, *fpOut;
    IMAGE *imgIn, *imgOut;

    if ((fpIn = fopen("venice.bmp", "r")) == NULL) {
        printf("ERROR -- could not open venice.bmp\n");
        return;
    }
    if (!readImage(fpIn, imgIn)) {
		printf("ERROR -- could not read the image.\n");
        return;
	}
    free(fpIn);
    imgOut->width = imgIn->width;
    imgOut->height = imgIn->height;
    imgOut->depth = imgIn->depth;
    imgOut->pixels = (PIXEL *)malloc(imgOut->width * imgOut->height * sizeof(PIXEL));
    if (imgOut->pixels == NULL) {
        printf("ERROR -- out of memory.\n");
        return;
    }
    for (i = 0; i < imgOut->width * imgOut->height; i++) {
        imgOut->pixels[i].r = colorInv((int)colorGrad(imgIn, i, 'r'));
        imgOut->pixels[i].g = colorInv((int)colorGrad(imgIn, i, 'g'));
        imgOut->pixels[i].b = colorInv((int)colorGrad(imgIn, i, 'b'));
    }
    if ((fpOut = fopen("venice-edge.bmp", "w")) == NULL) {
        printf("ERROR -- could not open venice-edge.bmp\n");
        return;
    }
    if (!saveImage(fpOut, imgOut)) {
		printf("ERROR -- could not write the image.\n");
        return;
	}
    free(fpOut);
}


int colorInv(int color)
{
    return 255 - color;
}


double colorGrad(IMAGE *img, int position, char ch)
{
    int up, down, left, right;
    PIXEL *upPixel    = getUpPixel(img, position);
    PIXEL *downPixel  = getDownPixel(img, position);
    PIXEL *rightPixel = getRightPixel(img, position);
    PIXEL *leftPixel  = getLeftPixel(img, position);

    if (upPixel == NULL || downPixel == NULL || rightPixel == NULL || leftPixel == NULL)
        return (double)0;

    switch (ch) {
        case 'r':
            up    = upPixel->r;
            down  = downPixel->r;
            left  = leftPixel->r;
            right = rightPixel->r;
            break;
        case 'g':
            up    = upPixel->g;
            down  = downPixel->g;
            left  = leftPixel->g;
            right = rightPixel->g;
            break;
        case 'b':
            up    = upPixel->b;
            down  = downPixel->b;
            left  = leftPixel->b;
            right = rightPixel->b;
            break;
        
    }
    return sqrt(pow((right - left) / 2, 2) + pow((up - down) / 2, 2));
}


PIXEL* getUpPixel(IMAGE *img, int position)
{
    if (position < img->width * (img->height - 1))
        return &(img->pixels[position + img->width]);
    else
        return NULL;
}


PIXEL* getDownPixel(IMAGE *img, int position)
{
    if (position > img->width)
        return &(img->pixels[position - img->width]);
    else
        return NULL;
}


PIXEL* getLeftPixel(IMAGE *img, int position)
{
    if (position % img->width)
        return &(img->pixels[position - 1]);
    else
        return NULL;
}


PIXEL* getRightPixel(IMAGE *img, int position)
{
    if (position % img->width != img->width - 1)
        return &(img->pixels[position + 1]);
    else
        return NULL;
}
