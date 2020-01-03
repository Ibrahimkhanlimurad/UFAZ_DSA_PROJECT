#ifndef _BMP_H_  // prevent recursive inclusion
#define _BMP_H_
#include <stdio.h>
#include <stdint.h>
#define FIRST_HEADER_SIZE 14
#define SECOND_HEADER_SIZE 56

typedef struct {
    uint32_t fileSize;
    uint32_t headerSize;
    uint32_t imageStartPosition;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint16_t numberOfColors;
    uint16_t numberOfBitsPerPixel;
} BMPHeader;

typedef struct {
    BMPHeader header;
    unsigned char* data;
} BMPImage;

BMPImage* readBmp(FILE* fp);
void readFirstHeader(BMPHeader* header, FILE* fp);
void readSecondHeader(BMPHeader* header, FILE* fp);
void checkHeader(BMPImage* image, FILE* fp);
void printBmp(BMPImage* image);
void freeBmp(BMPImage* image);

#endif  