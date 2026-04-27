#include <stdio.h>
#include <string.h>
#include "files.h"

void writeExtFile(const char *baseName, ExtRef *extRefs, int count)
{
    char  filename[256];
    FILE *fp;
    int   i;

    if (count == 0)
        return; /* no extern uses — do not create the file */

    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ext");

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    /* One line per reference site (not per definition) */
    for (i = 0; i < count; i++)
        fprintf(fp, "%s %04d\n", extRefs[i].name, extRefs[i].address);

    fclose(fp);
}
