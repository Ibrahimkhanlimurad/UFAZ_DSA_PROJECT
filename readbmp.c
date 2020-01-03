#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include "helper.h"
#include "readbmp.h"

BMPImage* readBmp(FILE* fp){
    BMPImage* image = malloc(sizeof(*image));
    if(!_check(image != NULL))
        _handleError("Not enough memory...", _MEMORY_ERROR, fp, image);

    rewind(fp);
    readFirstHeader(&image->header, fp);
    readSecondHeader(&image->header, fp);
    checkHeader(image, fp);

    return image;
}

void readFirstHeader(BMPHeader* header, FILE* fp){
    unsigned char firstHeader[FIRST_HEADER_SIZE+1];
    fgets((char*)firstHeader, FIRST_HEADER_SIZE+1, fp);
    if(!_check(strncmp((char*)firstHeader, "BM", 2) == 0))
        _softError("Invalid BMP header: unknown format...", _BMP_HEADER_ERROR);

    // printf("%u\n", firstHeader);
    header->fileSize = firstHeader[2] +
                    (firstHeader[3] << (unsigned) 8) +
                    (firstHeader[4] << (unsigned) 16) +
                    (firstHeader[5] << (unsigned) 24);

    header->imageStartPosition = firstHeader[10] +
                    (firstHeader[11] << (unsigned) 8) +
                    (firstHeader[12] << (unsigned) 16) +
                    (firstHeader[13] << (unsigned) 24);
}

void readSecondHeader(BMPHeader* header, FILE* fp){
    unsigned char secondHeader[SECOND_HEADER_SIZE+1];
    fgets((char*)secondHeader, SECOND_HEADER_SIZE+1, fp);
    // printf("%u\n", secondHeader);

    header->headerSize = secondHeader[0] +
                    (secondHeader[1] << (unsigned) 8) +
                    (secondHeader[2] << (unsigned) 16) +
                    (secondHeader[3] << (unsigned) 24);
    header->imageWidth = secondHeader[4] +
                    (secondHeader[5] << (unsigned) 8) +
                    (secondHeader[6] << (unsigned) 16) +
                    (secondHeader[7] << (unsigned) 24);
    header->imageHeight = secondHeader[8] +
                    (secondHeader[9] << (unsigned) 8) +
                    (secondHeader[10] << (unsigned) 16) +
                    (secondHeader[11] << (unsigned) 24);
    header->numberOfColors = secondHeader[12] +
                    (secondHeader[13] << (unsigned) 8);
    header->numberOfBitsPerPixel = secondHeader[14] +
                    (secondHeader[15] << (unsigned) 8);

}
 
void checkHeader(BMPImage* image, FILE* fp){
    BMPHeader* header = &image->header;

    if(!_check(header->fileSize != 0))
        _handleError("Invalid BMP header: file size is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->imageStartPosition != 0 || header->imageStartPosition <= header->fileSize))
        _handleError("Invalid BMP header: image start position is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->headerSize != 0))
        _handleError("Invalid BMP header: header size is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->headerSize != 0))
        _handleError("Invalid BMP header: image width is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->imageHeight != 0))
        _handleError("Invalid BMP header: image height is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->numberOfColors != 0))
        _handleError("Invalid BMP header: number of colors is wrong...", _BMP_HEADER_ERROR, fp, image);
    if(!_check(header->numberOfBitsPerPixel != 0))
        _handleError("Invalid BMP header: number of bits per pixel is wrong...", _BMP_HEADER_ERROR, fp, image);
};

void printBmp(BMPImage* image){
    BMPHeader* header = &image->header;
    printf("\n\n*****BMP Image Information*****\n\n");
    printf("~~BMP Header~~\n");
    printf("File size: %u\n", header->fileSize);
    printf("Header size: %u\n", header->headerSize);
    printf("Image Start Position size: %u\n", header->imageStartPosition);
    printf("Image Width: %u\n", header->imageWidth);
    printf("Image Height: %u\n", header->imageHeight);
    printf("Number of Color Planes: %u\n", header->numberOfColors);
    printf("Number of Bits per Pixel: %u\n", header->numberOfBitsPerPixel);
    // printf("~~BMP Data~~\n");
    // printf("Data: %s\n", image->data);

}

/*
 * Free all memory referred to by the given BMPImage.
 */
void freeBmp(BMPImage *image)
{
    free(image->data);
    free(image);
}
