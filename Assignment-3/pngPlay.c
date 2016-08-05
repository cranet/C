#include "pixutils.h"

int main(int argc, char *argv[]) {
  char *inputfile = 0, *outputfile = 0;
  float degrees = 0, grayFlag = 0;

  int count;
  for(count = 0; count < argc; count++) {
    /*Print arguments for reference*/
    //fprintf(stderr, "argv[%d] %s \n", count, argv[count]);

    /*Check for input*/
    if(strcmp(argv[count], "-i") == 0) {
      inputfile = argv[count + 1];
      count++;
    
    /*Check for output*/
    } else if(strcmp(argv[count], "-o") == 0) {
      outputfile = argv[count + 1];
      count++;
     
    /*Check for rotate*/ 
    } else if(strcmp(argv[count], "-r") == 0) {
      degrees = strtof(argv[count + 1], NULL);
      count++;
    
    /*Check for gray*/
    } else if(strcmp(argv[count], "-g") == 0) {
      grayFlag = 1;
      count++;
    }
  }

  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees)pixMap_rotate(p,degrees);
  if(grayFlag)pixMap_gray(p);
  pixMap_write(p,outputfile);
  pixMap_destroy(p);
  
  return 0;
}

