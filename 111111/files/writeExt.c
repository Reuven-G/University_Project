#include <stdio.h>
#include <string.h>
#include "files.h"




/* creates the .ext file */
void writeExtFile(const char *baseName, ExtRef *extRefs, int count)
{
    char  filename[256];
    FILE *fp;
    int   i;

	/* dont create file if no extern labels */
    if (count == 0)
        return;

	/* build the file */
    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ext");

	/* allow to write to the file */
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    for (i = 0; i < count; i++)
        fprintf(fp, "%s %04d\n", extRefs[i].name, extRefs[i].address);

    fclose(fp);
}
