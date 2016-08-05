//Copyright (c) 2005-2010 Lode Vandevenn
//lhhung modified cpp to c 

#include "lodepng.h"
#include <iostream>

//returns 0 if all went ok, non-0 if error
//output image is always given in RGBA (with alpha channel), even if it's a BMP without alpha channel
unsigned decodeBMP(unsigned char *image, unsigned w, unsigned h, unsigned char *bmp, int nbits)
{
  static const unsigned MINHEADER = 54; //minimum BMP header size
  
  if(bmp.size() < MINHEADER) return -1;
  if(bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
  unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
  //read width and height from BMP header
  w = bmp[18] + bmp[19] * 256;
  h = bmp[22] + bmp[23] * 256;
  //read number of channels from BMP header
  if(bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
  unsigned numChannels = bmp[28] / 8;

  //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
  //to make it a multiple of 4 bytes.
  unsigned scanlineBytes = w * numChannels;
  if(scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

  unsigned dataSize = scanlineBytes * h;
  if(bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

  image.resize(w * h * 4);

  /*
  There are 3 differences between BMP and the raw image buffer for LodePNG:
  -it's upside down
  -it's in BGR instead of RGB format (or BRGA instead of RGBA)
  -each scanline has padding bytes to make it a multiple of 4 if needed
  The 2D for loop below does all these 3 conversions at once.
  */
  for(unsigned y = 0; y < h; y++)
  for(unsigned x = 0; x < w; x++)
  {
    //pixel start byte position in the BMP
    unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
    //pixel start byte position in the new raw image
    unsigned newpos = 4 * y * w + 4 * x;
    if(numChannels == 3)
    {
      image[newpos + 0] = bmp[bmpos + 2]; //R
      image[newpos + 1] = bmp[bmpos + 1]; //G
      image[newpos + 2] = bmp[bmpos + 0]; //B
      image[newpos + 3] = 255;            //A
    }
    else
    {
      image[newpos + 0] = bmp[bmpos + 3]; //R
      image[newpos + 1] = bmp[bmpos + 2]; //G
      image[newpos + 2] = bmp[bmpos + 1]; //B
      image[newpos + 3] = bmp[bmpos + 0]; //A
    }
  }
  return 0;
}

void encodeBMP(unsigned char* bmp, unsigned char* image, int w, int h){
  //3 bytes per pixel used for both input and output.
  int inputChannels = 3;
  int outputChannels = 3;
  int k=0;
  //bytes 0-13
  bmp[k++]='B'; bmp[k++]='M'; //0: bfType
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; //2: bfSize; size not yet known for now, filled in later.
  bmp[k++]=0; bmp[k++]=0; //6: bfReserved1
  bmp[k++]=0; bmp[k++]=0; //8: bfReserved2
  bmp[k++]=54 % 256; bmp[k++]=54 / 256; bmp[k++]=0; bmp[k++]=0; //10: bfOffBits 54 header bytes

  //bytes 14-53
  bmp[k++]=40; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //14: biSize
  bmp[k++]=w % 256; bmp[k++]=w / 256; bmp[k++]=0; bmp[k++]=0; //18: biWidth
  bmp[k++]=h % 256; bmp[k++]=h / 256; bmp[k++]=0; bmp[k++]=0; //22: biHeight
  bmp[k++]=1; bmp[k++]=0; //26: biPlanes
  bmp[k++]=outputChannels * 8; bmp[k++]=0; //28: biBitCount
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //30: biCompression
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //34: biSizeImage
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //38: biXPelsPerMeter
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //42: biYPelsPerMeter
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //46: biClrUsed
  bmp[k++]=0; bmp[k++]=0; bmp[k++]=0; bmp[k++]=0;  //50: biClrImportant
  
  /*
  Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
  -BMP stores the rows inversed, from bottom to top
  -BMP stores the color channels in BGR instead of RGB order
  -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
  */

  int imagerowbytes = outputChannels * w;
  imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); //must be multiple of 4
  
  for(int y = h - 1; y >= 0; y--) //the rows are stored inversed in bmp
  {
    int c = 0;
    for(int x = 0; x < imagerowbytes; x++)
    {
      if(x < w * outputChannels)
      {
        int inc = c;
        //Convert RGB(A) into BGR(A)
        if(c == 0) inc = 2;
        else if(c == 2) inc = 0;
        bmp.push_back(image[inputChannels * (w * y + x / outputChannels) + inc]);
      }
      else bmp.push_back(0);
      c++;
      if(c >= outputChannels) c = 0;
    }
  }

  // Fill in the size
  bmp[2] = bmp.size() % 256;
  bmp[3] = (bmp.size() / 256) % 256;
  bmp[4] = (bmp.size() / 65536) % 256;
  bmp[5] = bmp.size() / 16777216;
}
