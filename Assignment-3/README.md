#Assignment 3
##TL;DR

Complete the code in pngPlay.c and pixutils.c to make the binary pngPlay which can rotate and gray out a 32 bit png. The following flags are to be supported and can be in any order:

	-i followed by input png
	-o followed by output png
	-g (optional) if present gray the png
	-r (optional) if present followed by degrees to rotate the png around the center of the png

Examples

`pngPlay -i input.png -o output.png -r 60.6 -g`

`pngPlay -o output.png -g -i input.png`

Additional details are available on Canvas and in comments in the files. A working Makefile is provided as are two sample 32 bit png's.
