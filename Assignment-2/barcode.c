#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define BARCODE_LENGTH 16 /*Default barcode length*/

#ifdef USE_MALLOC_WRAPPERS
#include "malloc_wrap.h"
#endif

int main_barcode(int argc, char *argv[])
{

	fprintf(stderr, "ABCDEF");	
	/*Assume lines are never longer than 512 char*/
	char line1[512], line2[512];
	char *inputfiles[2] = {0, 0}, *outputfile = 0;
	FILE *inputfp1 = 0, *inputfp2 = 0, *outputfp = stdout;
	
	/*Print error if args are not 3 or 5*/
	if(argc != 3 && argc != 5)
	{
		fprintf(stderr, "The command is bwa barcode <-o outputFile> R1file R2file\n");
		return 1;
	}

	/* Display command line arguments for parsing*/
	int count;
	fprintf(stderr, "\nCommand-line arguments: \n");
	for(count = 0; count < argc; count++)
	{
		fprintf(stderr, " argv[%d] %s \n", count, argv[count]);
	}

	/*Check for 3 arguments (no -o flag)*/
	if(argc == 3) 
	{
		/*Print for confirmation*/
		//fprintf(stderr, "argc = 3 \n");

		/*Assign the files*/
		inputfiles[0] = argv[1];
		inputfiles[1] = argv[2];

		/*Check for the files*/
		if(inputfiles[0] && inputfiles[1])
		{
			/*Check that the correct file is being read*/
			fprintf(stderr, "Reading in from %s \n", inputfiles[0]);
			fprintf(stderr, "Reading in from %s \n", inputfiles[1]);

			/*Open the file*/
			inputfp1 = fopen(inputfiles[0], "r");
			inputfp2 = fopen(inputfiles[1], "r");
			
			/*Move the first file down two lines to sync up with the second file*/
			fgets(line1, sizeof(line1), inputfp1);

			/*Parse the whole file*/
			int lineCounter = 0; /*Keeps track of total lines*/
			int dnaCounter = 0; /*Keeps track of DNA lines*/
			char delim[] = "\n"; /*Character to be removed*/
			while(fgets(line2, sizeof(line2), inputfp2) != NULL)
			{
				fgets(line1, sizeof(line1), inputfp1);
				lineCounter++;
				/*Get every first line*/
				if((lineCounter % 4) == 1)
				{
					/*Remove delim from string*/
					strtok(line2, delim);
					/*Add colon*/
					strncat(line2, ":", 1);
					/*Add DNA line*/
					strncat(line2, line1, BARCODE_LENGTH);
					/*Add new line char*/
					strncat(line2, delim, 1);
					/*Keep track of DNA count*/
					dnaCounter++;
				}
				fprintf(stdout, "%s", line2);
			}
			/*Check for correct file and print total lines and DNA lines*/
		// 	fprintf(stderr, "Reading in from %s \n", inputfiles[0]);
		// 	fprintf(stderr, "Total lines: %d \n", lineCounter);
		// fprintf(stderr, "DNA lines: %d \n", dnaCounter);
		}

	/*Check for 5 arguments and an -o flag*/ 
	} else if(argc == 5 && strcmp(argv[1], "-o") == 0)
	{
		/*Print for confirmation*/
		//fprintf(stderr, "argc = 5 \n");

		/*Assign the files*/
		outputfile = argv[2];
		inputfiles[0] = argv[3];
		inputfiles[1] = argv[4];

		/*Check for the files*/
		if(inputfiles[0] && inputfiles[1])
		{
			/*Check that the correct file is being read*/
			fprintf(stderr, "Reading in from %s \n", inputfiles[0]);
			fprintf(stderr, "Reading in from %s \n", inputfiles[1]);

			/*Open the file*/
			outputfp = fopen(outputfile, "w");
			inputfp1 = fopen(inputfiles[0], "r");
			inputfp2 = fopen(inputfiles[1], "r");
			
			/*Move the first file down two lines to sync up with the second file*/
			fgets(line1, sizeof(line1), inputfp1);

			/*Parse the whole file*/
			int lineCounter = 0; /*Keeps track of total lines*/
			int dnaCounter = 0; /*Keeps track of DNA lines*/
			char delim[] = "\n"; /*Character to be removed*/
			while(fgets(line2, sizeof(line2), inputfp2) != NULL)
			{
				fgets(line1, sizeof(line1), inputfp1);
				lineCounter++;
				/*Get every first line*/
				if((lineCounter % 4) == 1)
				{
					/*Remove delim from string*/
					strtok(line2, delim);
					/*Add colon*/
					strncat(line2, ":", 1);
					/*Add DNA line*/
					strncat(line2, line1, BARCODE_LENGTH);
					/*Add new line char*/
					strncat(line2, delim, 1);
					/*Keep track of DNA count*/
					dnaCounter++;
				}
				fprintf(outputfp, "%s", line2);
			}
			/*Check for correct file and print total lines and DNA lines*/
		// 	fprintf(stderr, "Reading in from %s \n", inputfiles[0]);
		// 	fprintf(stderr, "Total lines: %d \n", lineCounter);
		// fprintf(stderr, "DNA lines: %d \n", dnaCounter);
		}
	}

	fclose(inputfp1);
	fclose(inputfp2);
	if(outputfp != stdout)fclose(outputfp);
	return 0;
}
