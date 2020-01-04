#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include "writebmp.h"
#include "helper.h"

int currentPosition(int counter, unsigned x, unsigned y, BMPHeader* header){
	unsigned curY = counter/(header->imageHeight);
	unsigned curX = (header->imageWidth)-counter%(header->imageHeight);
	if(curX == x && curY == y) return (header->imageHeight)*(header->imageWidth)-counter;
	else return -1;
}

int getCurrentPosition(unsigned x, unsigned y, BMPHeader* header){
	if(x>=header->imageWidth || y>=header->imageHeight) return -1;

	int curPos = ((header->imageHeight)-y-1)*(header->imageWidth)+x;
	int bytePerPixel = (header->numberOfBitsPerPixel)/8;
	return (curPos*bytePerPixel+(header->imageStartPosition));
}

int getPadding(BMPHeader* header)
{
    return (4-(header->imageWidth * getBytePerPixel(header))%4)%4;
}


int getBytePerPixel(BMPHeader* header)
{
    return header->numberOfBitsPerPixel/8; //BITS_PER_BYTE
}

char* constructBMP(FILE* inputFile, BMPImage* image, Options* options, char* encodedText, int positionOffset, char* newData){
    rewind(inputFile);
    char ch;
    int count=0;
    unsigned x = options->position[0];
    int i=0, j=0;
    for (; i<image->header.fileSize;)
    {
        if (i>=positionOffset && encodedText[j] && x < image->header.imageWidth)
        {
            unsigned char* backgroundColor=(char*)malloc(4*sizeof(char));
            if(inputFile!=NULL) fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
            else memcpy(&backgroundColor[0], newData+i, getBytePerPixel(&image->header));

            if(encodedText[j] == '.'){
                memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=2;
                i += getBytePerPixel(&image->header)*2;
                count = getBytePerPixel(&image->header)*1;
            }else if(encodedText[j] == '-'){
                memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 2 * getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=4;
                i += getBytePerPixel(&image->header) * 4;
                count = getBytePerPixel(&image->header) * 3;
            }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
                memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=2;
                i += getBytePerPixel(&image->header)*2;
                count=getBytePerPixel(&image->header)*1;
            }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
                memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 2 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=4;
                i+=getBytePerPixel(&image->header)*4;
                count=getBytePerPixel(&image->header)*3;
                j++;
            }
            if(inputFile!=NULL) fseek( inputFile, count, SEEK_CUR );
            j++;
        }else{
            if(inputFile!=NULL) {
                ch=fgetc(inputFile);
                memcpy(newData + i, &ch, 1);
            }
            i++;
        }
    }

    if(encodedText[j]){
        options->position[0]=0;
        options->position[1]++;
        positionOffset=getCurrentPosition(options->position[0], options->position[1], &image->header);
        if(positionOffset==-1) {
            signBMP(image, options, newData);
            free(newData);
            _softError("\n\nPosition is out of image...", _USER_ERROR);
        };
        constructBMP(NULL, image, options, &encodedText[j], positionOffset, newData);  
    }

    if(inputFile!=NULL) fclose(inputFile);
}

void signBMP(BMPImage* image, Options* options, char* newData){
    if(_check(options->outputFile[0]!='\0')){
        FILE* signedFile = _openFile(options->outputFile, "wb", "ERROR: Could not open output file...");
        fwrite(newData, image->header.fileSize, 1, signedFile);
        _cleanUp(signedFile, image);
    }else
        fwrite(newData, image->header.fileSize, 1, stdout);
}