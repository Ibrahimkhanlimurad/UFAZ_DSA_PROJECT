#ifndef _WRITEBMP_H_  // prevent recursive inclusion
#define _WRITEBMP_H_
#include "readbmp.h"
#include "opt.h"

int currentPosition(int counter, unsigned x, unsigned y, BMPHeader* header);
int getCurrentPosition(unsigned x, unsigned y, BMPHeader* header);
int getPadding(BMPHeader* header);
int getBytePerPixel(BMPHeader* header);
char* constructBMP(FILE* inputFile, BMPImage* image, Options* options, char* encodedText, int positionOffset, char* newData);
void signBMP(BMPImage* image, Options* options, char* newData);

#endif