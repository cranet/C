#ifndef __pixutils_h
#define __pixutils_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
 #define M_PI 3.14159265
#endif

#define degreesToRadians(degrees) (degrees * M_PI / 180.0)

typedef struct rgba{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}rgba;

typedef struct{
	unsigned char *image;
 rgba **pixArray;
	unsigned int width;
	unsigned int height;
}pixMap;
#endif

//public constructors and destructor
pixMap *pixMap_init_filename(char *filename); //call pixMap_init and set the pixMap fields using lodepng
void pixMap_destroy (pixMap *p); //call pixMap_reset, free p and set pointer to zero

//public methods
int pixMap_write(pixMap *p,char *filename); //use lodepng to write to a file
void pixMap_write_bmp16(pixMap *p,char *filename); //convert to correct format
void pixMap_rotate (pixMap *p, float theta); //rotate the pixMap
void pixMap_gray(pixMap *p); //gray out the colors of the pixMap
