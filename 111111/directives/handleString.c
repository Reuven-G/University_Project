#include <stdio.h>
#include "directives.h"




/* extract characters from inside quotas */
void handleString(char *line, int *DC, int dataImage[])
{
    int i = 0;

    /* skip blank */
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    /* expect opening quote */
    if (line[i] != '"')
    {
        fprintf(stderr,
                "Error: '.string' - expected '\"' but got '%c'\n", line[i]);
        return;
    }
    i++; /* skip opening quote */

    /* store characters temporarly */
    while (line[i] != '"' && line[i] != '\0' && line[i] != '\n')
    {
        dataImage[*DC] = (unsigned char)line[i];
        (*DC)++;
        i++;
    }

    if (line[i] != '"')
    {
        fprintf(stderr, "Error: '.string' - missing closing '\"'\n");
    }

    dataImage[*DC] = 0;
    (*DC)++;
}
