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

// void signBMP(FILE* inputFile, FILE* signedFile, BMPImage* image, Options* options, char* encodedText, int positionOffset){
// 	rewind(inputFile);
//     char ch;
//     int count=0;

//     for (int i=0, j=0; i<image->header.fileSize;)
//     {
//         if (i>=positionOffset && encodedText[j])
//         {
//             unsigned char* backgroundColor=(char*)calloc(1, 4*sizeof(char));
//             fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
//             if(encodedText[j] == '.'){
//                 fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 i+=getBytePerPixel(&image->header)*2;
//                 count=getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == '-'){
//                 fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&options->color[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 i+=getBytePerPixel(&image->header)*4;
//                 count=getBytePerPixel(&image->header)*3;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 i+=getBytePerPixel(&image->header)*2;
//                 count=getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 fwrite(&backgroundColor[0], getBytePerPixel(&image->header), 1, signedFile);
//                 i+=getBytePerPixel(&image->header)*4;
//                 count=getBytePerPixel(&image->header)*3;
//                 fseek( inputFile, count, SEEK_CUR );
//                 j++;
//             }
//             j++;
//         }else{
//             ch=fgetc(inputFile);
//             fputc(ch, signedFile);    
//             i++;
//         }
//     }
// }


// void signBMP(FILE* inputFile, FILE* signedFile, BMPImage* image, Options* options, char* encodedText, int positionOffset){

//     rewind(inputFile);
//     char ch;
//     int count=0;
//     char* newData = (char*) calloc(1, image->header.fileSize);
//     for (int i=0, j=0; i<image->header.fileSize;)
//     {
//         if (i>=positionOffset && encodedText[j])
//         {
//             unsigned char* backgroundColor=(char*)malloc(4*sizeof(char));
//             fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
//             if(encodedText[j] == '.'){
//                 memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 i += getBytePerPixel(&image->header)*2;
//                 count = getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == '-'){
//                 memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 2 * getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 i += getBytePerPixel(&image->header) * 4;
//                 count = getBytePerPixel(&image->header) * 3;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
//                 memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 i += getBytePerPixel(&image->header)*2;
//                 count=getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
//                 memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 2 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 i+=getBytePerPixel(&image->header)*4;
//                 count=getBytePerPixel(&image->header)*3;
//                 fseek( inputFile, count, SEEK_CUR );
//                 j++;
//             }
//             j++;
//         }else{
//             ch=fgetc(inputFile);
//             memcpy(newData + i, &ch, 1);
//             i++;
//         }
//     }
    
//     fwrite(newData, image->header.fileSize, 1, signedFile);
// }



// void signBMP(FILE* inputFile, BMPImage* image, Options* options, char* encodedText, int positionOffset, char* newData){
//     rewind(inputFile);
//     char ch;
//     int count=0;
//     unsigned x = options->position[0];
//     int i=0, j=0;
//     for (; i<image->header.fileSize;)
//     {
//         if (i>=positionOffset && encodedText[j] && x < image->header.imageWidth)
//         {
//             // printf("%s\n", encodedText);
//             // printf("%d\n", i);
//             unsigned char* backgroundColor=(char*)malloc(4*sizeof(char));
//             fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
//             if(encodedText[j] == '.'){
//                 memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 x+=2;
//                 i += getBytePerPixel(&image->header)*2;
//                 count = getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == '-'){
//                 memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 2 * getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 x+=4;
//                 i += getBytePerPixel(&image->header) * 4;
//                 count = getBytePerPixel(&image->header) * 3;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
//                 memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 x+=2;
//                 i += getBytePerPixel(&image->header)*2;
//                 count=getBytePerPixel(&image->header)*1;
//                 fseek( inputFile, count, SEEK_CUR );
//             }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
//                 memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 2 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
//                 x+=4;
//                 i+=getBytePerPixel(&image->header)*4;
//                 count=getBytePerPixel(&image->header)*3;
//                 fseek( inputFile, count, SEEK_CUR );
//                 j++;
//             }
//             j++;
//         }else{
//             ch=fgetc(inputFile);
//             memcpy(newData + i, &ch, 1);
//             i++;
//         }
//     }


//     // FILE* signedFile = tmpfile();
//     FILE* signedFile = _openFile(options->outputFile, "wb+", "ERROR: Could not open output file...");
//     fwrite(newData, image->header.fileSize, 1, signedFile);

//     if(encodedText[j]){
//         options->position[0]=0;
//         options->position[1]++;
//         positionOffset=getCurrentPosition(options->position[0], options->position[1], &image->header);
//         signBMP(signedFile, image, options, &encodedText[j], positionOffset, newData);  
//     }
// }



void signBMP(FILE* inputFile, BMPImage* image, Options* options, char* encodedText, int positionOffset, char* newData){
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
            // if (*newData == 0)
            // {
            //     fread(&backgroundColor[0], getBytePerPixel(&image->header), 1, inputFile);
            // }else{
                
            // }
            if(encodedText[j] == '.'){
                memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=2;
                i += getBytePerPixel(&image->header)*2;
                count = getBytePerPixel(&image->header)*1;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == '-'){
                memcpy(newData + i, &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 2 * getBytePerPixel(&image->header), &options->color[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=4;
                i += getBytePerPixel(&image->header) * 4;
                count = getBytePerPixel(&image->header) * 3;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == ' ' && encodedText[j+1] != ' '){
                memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=2;
                i += getBytePerPixel(&image->header)*2;
                count=getBytePerPixel(&image->header)*1;
                fseek( inputFile, count, SEEK_CUR );
            }else if(encodedText[j] == ' ' && encodedText[j+1] == ' '){
                memcpy(newData + i, &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 2 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                memcpy(newData + i + 3 * getBytePerPixel(&image->header), &backgroundColor[0], getBytePerPixel(&image->header));
                x+=4;
                i+=getBytePerPixel(&image->header)*4;
                count=getBytePerPixel(&image->header)*3;
                fseek( inputFile, count, SEEK_CUR );
                j++;
            }
            j++;
        }else{
            ch=fgetc(inputFile);
            memcpy(newData + i, &ch, 1);
            i++;
        }
    }


    FILE* signedFile;
    if(_check(options->outputFile[0]!='\0')) signedFile = _openFile(options->outputFile, "wb+", "ERROR: Could not open output file...");
    else signedFile = _openFile("draft.bmp", "wb+", "ERROR: Could not open output file...");

    // fwrite(newData, image->header.fileSize, 1, signedFile);

    if(encodedText[j]){
        options->position[0]=0;
        options->position[1]++;
        positionOffset=getCurrentPosition(options->position[0], options->position[1], &image->header);
        signBMP(signedFile, image, options, &encodedText[j], positionOffset, newData);  
    }

    fclose(signedFile);
    fclose(inputFile);

    // return newData;
}


void signOUT(){
    FILE* signedFile = _openFile("draft.bmp", "rb+", "ERROR: Could not open output file...");
    fseek(signedFile, 0L, SEEK_END);
    int sz = ftell(signedFile);
    rewind(signedFile);
    char c; 
    c = fgetc(signedFile); 
    for (int i=0; i<sz;i++)
    { 
        fputc(c, stdout); 
        c = fgetc(signedFile); 
    } 

    fclose(signedFile);
    remove("draft.bmp");
    unlink("draft.bmp");
}