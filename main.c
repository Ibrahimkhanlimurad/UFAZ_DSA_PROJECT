#include "opt.h"
#include "helper.h"
#include "morse.h"
#include "readbmp.h"
#include "writebmp.h"

int main(int argc, char *argv[])
{
	Options* options = getOptions(argc, argv);
    char* textToEncode = getTextToEncode(options);
	char* encodedText = morseGenerator(textToEncode);
    FILE* inputFile = _openFile(options->inputFile, "rb+", "ERROR: Could not open input file, please check if you have input file on input folder...");
    BMPImage* image = readBmp(inputFile);
    int positionOffset=getCurrentPosition(options->position[0], options->position[1], &image->header);
    convertColorToLittleEndian(options, image);
    // printAll(options, image, textToEncode, encodedText, positionOffset);
    logAll(options, image, textToEncode, encodedText, positionOffset);
    char* newData = (char*) calloc(1, image->header.fileSize);
    signBMP(inputFile, image, options, encodedText, positionOffset, newData);
    free(newData);
    if(_check(options->outputFile[0]=='\0'))
        signOUT();

    // if(_check(options->outputFile[0]!='\0')){
    //     FILE* signedFile = _openFile(options->outputFile, "wb+", "ERROR: Could not open output file...");
    //     signBMP(inputFile, image, options, encodedText, positionOffset, newData);
    // }else
    //     signBMP(inputFile, image, options, encodedText, positionOffset, newData);
    
	return 0;
}

