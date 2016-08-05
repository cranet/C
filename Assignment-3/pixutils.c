#include "pixutils.h"
#include "lodepng.h"

/*Private methods*/
static pixMap *pixMap_init(); /*allocate memory for pixMap object, set variables to zero, and return a pointer to the object*/
static void pixMap_reset();  /*free the allocated memoray and set to zero but do not free memory for pixMap*/
static void pixMap_copy(pixMap *dest,pixMap *source); /*copy one pixmap to another*/
static int pixMap_read(pixMap *p,char *filename); /*read in a pixmap from file*/

/*Constructor*/
static pixMap* pixMap_init() {

	/*Allocate memory for pixMap*/
	pixMap *p;
	p = malloc(sizeof(pixMap));

	/*Set variables to zero*/
	p->image = 0;
	p->pixArray = 0;
	p->width = 0;
	p->height = 0;

	/*Return pointer to object*/
	return  p;
}	

/*Creates the pixMap object*/
pixMap* pixMap_init_filename(char *filename) {

	/*Create the pixMap object*/
	pixMap *a = pixMap_init();

	/*Fill fields*/
	pixMap_read(a, filename);	

	/*Return pointer*/
	return a;
}

/*Reads the image from a file*/
static int pixMap_read(pixMap *p, char *filename) {
	
	/*Read and allocate image*/
	lodepng_decode32_file(&p->image, &p->width, &p->height, filename);

	/*Allocate pixArray*/
	p->pixArray = malloc(p->height * sizeof(p));
	
	/*Set and fill array*/
	p->pixArray[0] = p->image;
	int i;
	for(i = 1; i < p->height; i++) {
		p->pixArray[i] = malloc(sizeof(p->width) * sizeof(p));
		p->pixArray[i] = p->pixArray[i - 1] + p->width;
	}

	return 0;
}	

/*Creates a copy of the image*/
static void pixMap_copy(pixMap *dest, pixMap *source) {

	/*If source is zero, reset*/
	if(source == NULL) {
		pixMap_reset(dest);
	
	/*If source image is not zero*/
	} else if(source) {
		/*Destination width/height is different from source width/height*/
		if(dest->width != source->width || dest->height != source->height) {
			/*Width*height is different*/
			if(dest->width * dest->height != source->width * source->height) {
				/*Destination is null, allocate memory*/
				if(dest->image == NULL) {
					dest->image = malloc(source->height * source->width * sizeof(rgba));
				/*Not null, reallocate memory*/
				} else {
					dest->image = realloc(dest->image, sizeof(dest) * dest->height);
				}
			} 

			/*Destination height and source height are different*/
			if(dest->height != source->height) {
				/*Destination pixArray is null, allocate memory*/
				if(dest->pixArray == NULL) {
					dest->pixArray = malloc(source->height * sizeof(source));
				/*Not null, reallocate memory*/
				} else {
					dest->pixArray = realloc(dest->pixArray, sizeof(dest) * dest->height);
				}
			}

			/*Fill array*/
			dest->pixArray[0] = dest->image;
			int i;
			for(i = 1; i < source->height; i++) {
				dest->pixArray[i] = dest->pixArray[i - 1] + source->width;
			}
		}

		/*Mem copy from source image to destination image*/
		memcpy(dest->image, source->image, source->height * source-> width * sizeof(rgba));
		
		/*Set destination width/height*/
		dest->width = source->width;
		dest->height = source->height;
	}
}

/*Frees memory and resets image values*/
static void pixMap_reset(pixMap *p) {

	/*Free all the memory*/
	free(&p->image);
	free(p->pixArray);

	/*Set all fields to zero*/
	p->width = 0;
	p->height = 0;
}	

/*Destroys the image*/
void pixMap_destroy(pixMap *p) {
	
	/*Reset p and free p object*/
	pixMap_reset(p);
	p = 0;
	free(p);
}

/*Rotates the image a specified number of degrees*/
void pixMap_rotate (pixMap *p, float theta) {

	/*Temp pixMap*/
	pixMap *temp = pixMap_init();

	/*Copy p to temp*/
	pixMap_copy(temp, p);

	/*Set values in the image to zero*/
	memset(temp->image, '0', sizeof(rgba) * p->width * p->height);

	/*Calculate middle (ox, oy)*/
	float ox = p->width / 2;
	float oy = p->height / 2;

	/*Calculate sine/cosine*/
	float s = sin(degreesToRadians(theta));
	float c = cos(degreesToRadians(theta));

	/*Traverse all pixels in p*/
	int y; /*height*/
	for(y = 0; y < p->height; y++) {
		int x; /*width*/
		for(x = 0; x < p->width; x++) {
			/*Calculate rotated coordinates, round and cast to int*/
			float frotx = (c * (x - ox) - s * (oy - y) + ox) + 0.5;
			int rotx = frotx;

			float froty = (-(s * (x - ox) + c * (oy - y) - oy)) + 0.5;
			int roty = froty;

			/*If rotation is within boundary, copy old pixel to temp*/
			if(rotx < p->width && rotx > 0 && roty < p->height && roty > 0) {
				memcpy(temp->pixArray[roty] + rotx, p->pixArray[y] + x, sizeof(rgba));
			}
		}	
	}

	/*Copy temp to original, destroy temp*/
	pixMap_copy(p, temp);
	pixMap_destroy(temp);	
}	

/*Outputs the image in grayscale*/
void pixMap_gray (pixMap *p) {

	/*Traverse all pixels in p*/
	int i;
	for(i = 0; i < p->height; i++) {
		int j;
		for(j = 0; j < p->width; j++) {

			/*Average value of rgb, round and cast*/
			float average = (p->pixArray[i][j].r + p->pixArray[i][j].g + p->pixArray[i][j].b) / 3;
			int gray = average + 0.5;

			/*Assign rounded value to rgb*/
			p->pixArray[i][j].r = gray;
			p->pixArray[i][j].g = gray;
			p->pixArray[i][j].b = gray;
		}
	}	
}

/*Writes the image to a new file*/
int pixMap_write(pixMap *p, char *filename) {

 	/*Write out to the filename*/
 	lodepng_encode32_file(filename, p->image, p->width, p->height);
	return 0;
}	 
