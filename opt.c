#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "helper.h"
#include "opt.h"

Options* newOptions(char* inputFile, char* outputFile, char* text, unsigned xPos, unsigned yPos, uint8_t hasDate, unsigned char* color){
    Options* newOptions = (Options*)malloc(sizeof(Options));
    newOptions->inputFile = inputFile;
    newOptions->outputFile = outputFile;
    newOptions->text=text;
    newOptions->position[0]=xPos;
    newOptions->position[1]=yPos;
    newOptions->hasDate=hasDate;
    strncpy(newOptions->color, color, 4);

    return newOptions;
}

void printOptions(Options* options){
    printf("\n\n*****Options Information*****\n\n");
    printf("Input file: %s\n", options->inputFile);
    printf("Output file: %s\n", options->outputFile);
    printf("Sign text: %s\n", options->text);
    printf("Position: {%u, %u}\n", options->position[0], options->position[1]);
    printf("Color as hex bytes: (%x, %x, %x, %x)\n", options->color[0], options->color[1], options->color[2], options->color[3]);
    printf("Color as numbers: (%u, %u, %u, %u)\n", options->color[0], options->color[1], options->color[2], options->color[3]);
    printf("Has Date?  -%s\n", options->hasDate ? "Yes" : "No");
}

Options* getOptions(int argc, char *argv[]){
    char* inputFile=(char*)malloc(100*sizeof(char));
    strcpy(inputFile, "../input/");
    if(argc >= 2 && argv[1] && strcat(inputFile, argv[1]) && strcmp(argv[1], "-h")!=0 && strcmp(argv[1], "-help")!=0)
        _openFile(inputFile, "rb+", "ERROR: Could not open input file, please check if you have input file on input folder...");
    else{
        if (argv[1] && (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "-help")==0)) optind--;
        else{
            fprintf(stderr, "For usage information: ./%s -h/-help", argv[0]);
            exit(_USER_ERROR);
        }
    }

    static struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "text", required_argument, NULL, 't' },
        { "date", no_argument, NULL, 'd' },
        { "pos", required_argument, NULL, 'p' },
        { "o", required_argument, NULL, 'o' },
        { "color", required_argument, NULL, 'c'},
        { 0,         0,           0, 0 }
    };

    char* outputFile=(char*)malloc(100);
    outputFile[0] = '\0';
    char* text=(char*)malloc(200);
    text[0] = '\0';
    unsigned char* color=(char*)malloc(4);
    color[0] = '\0';
    unsigned xPos=0;
    unsigned yPos=0;
    uint8_t hasDate=0;;

    char ch;
    optind++;
    char error[1000];   
    while (optind<argc) {
        if((ch = getopt_long_only(argc, argv, "ht:dp:o:", longopts, NULL)) != -1){
            switch (ch) {
                case 'h':
                    fprintf(stderr, "\nUsage: ./%s [filename][options]\nOptions:\n\t\t[filename]\t\t-\t(obligatory)\n\t\t[-text <sign-text>]\t-\t(optional)\n\t\t"
                        "[-date]\t\t\t-\t(optional)\n\t\t[-color <hex-color>]\t-\t(optional)\n\t\t[-pos <x, y>]\t\t-\t(optional)\n\t\t[-o <output-filename>]\t-\t(mandatory)"
                        "\n\n\t\t[-h or -help]\t\t-\t(Print usage)", argv[0]);
                    exit(_USER_ERROR);
                case 't':
                    for(int i = 0; optarg[i]; i++){
                        text[i] = tolower(optarg[i]);
                        text[i+1] = '\0';
                    }
                    break;
                case 'd':
                    hasDate = 1;
                    break;
                case 'p':
                    sscanf(optarg, "%u,%u", &xPos, &yPos);
                    break;
                case 'o':
                    strcpy(outputFile, optarg);
                    break;
                case 'c':
                    sscanf(optarg, "%2hhx%2hhx%2hhx", &color[0], &color[1], &color[2]);
                    color[3] = 0;
                    break;
                case ':':
                    fprintf(stderr, "Option '%s' has missing argument", argv[optind - 1]);
                    exit(_USER_ERROR);
                    break;
                case '?':
                    if (optopt == 0) 
                        fprintf(stderr, "Unknown option '%s'\n", argv[optind - 1]);
                    else 
                        fprintf(stderr, "Error parsing option '%s'\n", argv[optind - 1]);
                    fprintf(stderr, "Run ./%s -help for usage information...\n", argv[0]);
                    exit(_USER_ERROR);
                    break;
            }
        }
        else{
            fprintf(stderr, "\nWARNING: Option '%s' is unknown. Ignoring.", argv[optind]);
            optind++;
        }
    }

    
    if(_check(outputFile[0]=='\0')){
        char* temp = (char*)malloc(200);
        strcpy(temp, "../output/");
        strcat(temp, outputFile);
        strcpy(outputFile, temp);
    }

    Options* options = newOptions(inputFile, outputFile, text, xPos, yPos, hasDate, color);
    return options;
}
