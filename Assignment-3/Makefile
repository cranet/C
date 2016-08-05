CC=gcc
LD=gcc
CFLAGS=-I. -ggdb -std=c99 -Wall -O2
LDFLAGS= -lm
pngPlay: pngPlay.o pixutils.h pixutils.o lodepng.o
	$(LD) -o pngPlay pngPlay.o pixutils.h pixutils.o lodepng.o $(LDFLAGS)
	
clean:
	rm *.o

