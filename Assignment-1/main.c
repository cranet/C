#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "insertionSort.h"
#include "sort1.h"
#include "sort2.h"

#define VECTORSIZE 100000

int compareVectors(int * vector1, int * vector2, int size);

int main()
{
	int r, i, originalVector[VECTORSIZE], sortedVector[VECTORSIZE], testVector[VECTORSIZE];
	/*seed random number*/
	srand(1234);
	clock_t start, diff;
	
 /*initialize random number*/
	for (i = 0; i < VECTORSIZE; i++) 
	{
		originalVector[i] = rand() % 10000;
	}

	/*copy vector to sorted vector*/
	memmove(sortedVector, originalVector, sizeof(originalVector));

 /*insertion sort*/
 start = clock(); /*start timer*/
	insertion_sort(sortedVector, VECTORSIZE); 
 printf ("Insertion sort took %d ms\n", (clock()-start) * 1000 / CLOCKS_PER_SEC);
 
 /*sort1 copy vector to test vector*/
	memmove(testVector, originalVector, sizeof(originalVector));
	start = clock(); /*start timer*/
	sort1(testVector, VECTORSIZE);
	printf ("Bubble sort took %d ms\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
 
 /*check that it is sorted*/
 if(!compareVectors(sortedVector, testVector, VECTORSIZE))
 {
		printf("Bubble sort failed\n");
		return 0;
 }

 /*sort2 copy vector to test vector*/
	memmove(testVector, originalVector, sizeof(originalVector));
	start = clock(); /*start timer*/
	sort2(testVector, VECTORSIZE);
	printf("Selection sort took %d ms\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
 
 /*check that it is sorted*/
 if(!compareVectors(sortedVector, testVector, VECTORSIZE))
 {
		printf("Selection failed\n");
		return 0 ;
	}
	return 1;
}

int compareVectors(int * vector1, int * vector2, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
	 if(vector1[i] != vector2[i]) return(0);
	} 
	return(1);
}	 

