#include <stdlib.h>
#include "writebmp.h"
#include "helper.h"

int currentPosition(int counter, unsigned x, unsigned y, BMPHeader* header){
	unsigned curY = counter/(header->imageHeight);
	unsigned curX = (header->imageWidth)-counter%(header->imageHeight);
	if(curX == x && curY == y) return (header->imageHeight)*(header->imageWidth)-counter;
	else return -1;
}

int getCurrentPosition(unsigned x, unsigned y, BMPHeader* header){
	if(x>=header->imageWidth || y>=header->imageHeight) _softError("\n\nPosition is out of image...", _USER_ERROR);

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

void signBMP(FILE* inputFile, FILE* signedFile, BMPImage* image, Options* options, char* encodedText, int positionOffset){
	rewind(inputFile);
    char ch;
    int count=0;
    for (int i=0, j=0; i<image->header.fileSize;)
    {
        if (i>=positionOffset && encodedText[j])
        {
            unsigned char* backgroundColor=(char*)malloc(4*sizeof(char));
            fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
            if(encodedText[j] == '.'){
                fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                i+=getBytePerPixel(&image->header)*2;
                count=getBytePerPixel(&image->header)*1;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == '-'){
                fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                i+=getBytePerPixel(&image->header)*4;
                count=getBytePerPixel(&image->header)*3;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                i+=getBytePerPixel(&image->header)*2;
                count=getBytePerPixel(&image->header)*1;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
                i+=getBytePerPixel(&image->header)*4;
                count=getBytePerPixel(&image->header)*3;
                fseek( inputFile, count, SEEK_CUR );
                j++;
            }
            j++;
        }else{
            ch=fgetc(inputFile);
            fputc(ch, signedFile);    
            i++;
        }
    }
}
