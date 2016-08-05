#compiler
CC=gcc
#linker
LD=gcc
#include file directory
CFLAGS=-I.

#linker rule
testSort: main.o insertionSort.o sort1.o sort2.o
#	gcc -o testSort main.o insertionSort.o
	$(LD) -o testSort main.o insertionSort.o sort1.o sort2.o
	
#compiler rules
main.o: main.c
#gcc -c -I. main.c
	$(CC) -c $(CFLAGS) main.c	
	 
insertionSort.o: insertionSort.c
	$(CC) -c $(CFLAGS) insertionSort.c

sort1.o: sort1.c
	$(CC) -c $(CFLAGS) sort1.c

sort2.o: sort2.c
	$(CC) -c $(CFLAGS) sort2.c 

#clean up procedure		
clean:
	rm *.o

