#ifndef _OPT_H_  // prevent recursive inclusion
#define _OPT_H_
#include <stdint.h>

typedef struct {
	char* inputFile;
	char* outputFile;
	char* text;
	unsigned position[2];
	uint8_t hasDate;
	unsigned char color[4];
} Options;

Options* newOptions(char* inputFile, char* outputFile, char* text, unsigned xPos, unsigned yPos, uint8_t hasDate, unsigned char* color);
void printOptions(Options* options);
Options* getOptions(int argc, char *argv[]);

#endif