#ifndef _HELPER_H_  // prevent recursive inclusion
#define _HELPER_H_
#include "opt.h"
#include "readbmp.h"
#include <stdint.h>

#define _TRUE 1
#define _FALSE 0
#define _USER_ERROR 20
#define _FILE_ERROR 30
#define _MEMORY_ERROR 40
#define _BMP_ERROR 50
#define _BMP_HEADER_ERROR 51
#define _LOGGING_ERROR 60

void _softError(char* message, int code);
void _handleError(char* errorMessage, int code, FILE* fp, BMPImage* image);
void _cleanUp(FILE *fp, BMPImage *image);
uint8_t _check(uint8_t condition);
char* getDate();
char* getTextToEncode(Options* options);
void convertColorToLittleEndian(Options* options, BMPImage* image);
void printAll(Options* options, BMPImage* image, char* textToEncode, char* encodedText, int positionOffset);
void logAll(Options* options, BMPImage* image, char* textToEncode, char* encodedText, int positionOffset);
FILE* _openFile(const char* filename, const char* mode, char* errorMessage);

#endif