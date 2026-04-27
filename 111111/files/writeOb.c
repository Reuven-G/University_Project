#include <stdio.h>
#include <string.h>
#include "files.h"

#define IC_START 100

void writeObFile(const char *baseName,
                 int *codeImage, int icSize,
                 int *dataImage, int dcSize)
{
    char filename[256];
    FILE *fp;
    int   i;
    int   addr;

    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ob");

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    /* Header line */
    fprintf(fp, "  %d %d\n", icSize, dcSize);

    /* Code image words */
    addr = IC_START;
    for (i = 0; i < icSize; i++)
    {
        /* Address: 4 digits, value: 3 hex digits (12 bits = max 0xFFF) */
        fprintf(fp, "%04d %03X\n", addr, codeImage[i] & 0xFFF);
        addr++;
    }

    /* Data image words (addresses follow immediately after code) */
    for (i = 0; i < dcSize; i++)
    {
        fprintf(fp, "%04d %03X\n", addr, dataImage[i] & 0xFFF);
        addr++;
    }

    fclose(fp);
}
