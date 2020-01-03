#ifndef _WRITEBMP_H_  // prevent recursive inclusion
#define _WRITEBMP_H_
#include "readbmp.h"
#include "opt.h"

int currentPosition(int counter, unsigned x, unsigned y, BMPHeader* header);
int getCurrentPosition(unsigned x, unsigned y, BMPHeader* header);
int getPadding(BMPHeader* header);
int getBytePerPixel(BMPHeader* header);
void signBMP(FILE* inputFile, FILE* signedFile, BMPImage* image, Options* options, char* encodedText, int positionOffset);
void signOUT(FILE* inputFile, BMPImage* image, Options* options, char* encodedText, int positionOffset);

#endif