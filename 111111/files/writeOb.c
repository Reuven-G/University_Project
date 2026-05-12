#include <stdio.h>
#include <string.h>
#include "files.h"

#define IC_START 100




/* the creation of the .ob file */
void writeObFile(const char *baseName, int *codeImage, char *areImage, int icSize, int *dataImage, int dcSize)
{
    char  filename[256];
    FILE *fp;
    int   i;
    int   addr;

	/* build the file */
    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ob");

	/* allow to write to the file */
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    /* header */
    fprintf(fp, "    %d %d\n", icSize, dcSize);

    /* A.R.E letters */
    addr = IC_START;
    for (i = 0; i < icSize; i++)
    {
        fprintf(fp, "%04d %03X %c\n", addr, codeImage[i] & 0xFFF, areImage[i]);
        addr++;
    }

    /* data image */
    for (i = 0; i < dcSize; i++)
    {
        fprintf(fp, "%04d %03X A\n", addr, dataImage[i] & 0xFFF);
        addr++;
    }

    fclose(fp);
}

