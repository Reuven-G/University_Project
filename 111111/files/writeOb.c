#include <stdio.h>
#include <string.h>
#include "files.h"

#define IC_START 100

/* ARE bit values stored in bits 1-0 of each word */
static char areChar(int word)
{
    switch (word & 0x3)
    {
        case 0: return 'A';
        case 1: return 'R';
        case 2: return 'E';
        default: return '?';
    }
}

void writeObFile(const char *baseName,
                 int *codeImage, int icSize,
                 int *dataImage, int dcSize)
{
    char filename[256];
    FILE *fp;
    int   i, addr;

    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ob");

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    fprintf(fp, "    %d %d\n", icSize, dcSize);

    addr = IC_START;
    for (i = 0; i < icSize; i++)
    {
        fprintf(fp, "%04d %03X\n", addr, codeImage[i] & 0xFFF);
        addr++;
    }

    for (i = 0; i < dcSize; i++)
    {
        fprintf(fp, "%04d %03X\n", addr, dataImage[i] & 0xFFF);
        addr++;
    }

    fclose(fp);
}
