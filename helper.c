#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "readbmp.h"
#include "helper.h"
#include "writebmp.h"

void _softError(char* message, int code){
    fprintf(stderr, "\n\n%s\n", message);
    exit(code);
}

void _handleError(char* errorMessage, int code, FILE *fp, BMPImage* image)
{
    fprintf(stderr, "\n\nERROR: %s\n", errorMessage);
    _cleanUp(fp, image);
    exit(code);
}

void _cleanUp(FILE *fp, BMPImage *image)
{
    if (fp != NULL) fclose(fp);
    freeBmp(image);
}

uint8_t _check(uint8_t condition)
{
    uint8_t is_valid = _TRUE;
    if(!condition) is_valid = _FALSE;
    return is_valid;
}

char* getDate(){
	  time_t rawtime;
 	  struct tm* timeinfo;
  	time (&rawtime);
  	timeinfo = localtime(&rawtime);
  	char* curTime = asctime(timeinfo);
  	for (int i=0; curTime[i]!='\0'; i++) curTime[i]=tolower(curTime[i]);

  	return curTime;
}

char* getTextToEncode(Options* options){
    char* textToEncode = (char*)malloc(500); 
    textToEncode[0]='\0';

    if (_check(strcmp(options->text, "") != 0)) strcat(textToEncode, options->text);
    if(_check(options->hasDate == 1)) {
      strcat(textToEncode, " ");
      strcat(textToEncode, getDate());
    } 

    return textToEncode;  
}

void convertColorToLittleEndian(Options* options, BMPImage* image){
    unsigned char f = options->color[0];
    unsigned char s = options->color[1];
    unsigned char t = options->color[2];
    unsigned char fo = options->color[3];

    if(getBytePerPixel(&image->header) == 3){
        options->color[0] = t;
        options->color[1] = s;
        options->color[2] = f;
    }else if(getBytePerPixel(&image->header) == 4){
        options->color[0] = fo;
        options->color[1] = t;
        options->color[2] = s;  
        options->color[3] = f;  
    }
}

void printAll(Options* options, BMPImage* image, char* textToEncode, char* encodedText, int positionOffset){
    printOptions(options);
    printBmp(image);
    printf("\n\n\n*****~~~~~~~~~~~*****\n");
    printf("\n*****Sign Text*****\n");
    printf("%s\n", textToEncode);
    printf("\n*****Sign Morse*****\n");
    printf("%s\n", encodedText);
    printf("\n*****Position Offset*****\n");
    printf("%d\n", positionOffset);
}

/*
 * Open file. In case of error, print message and exit.
 */
FILE* _openFile(const char* filename, const char* mode, char* errorMessage)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) 
    	_softError(errorMessage, _FILE_ERROR);
    return fp;
}