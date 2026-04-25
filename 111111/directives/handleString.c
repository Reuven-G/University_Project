#include <stdio.h>
#include "directives.h"

void handleString(char *line, int *DC, int dataImage[])
{
    int i = 0;

    /* Skip leading whitespace */
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    /* Expect opening quote */
    if (line[i] != '"')
    {
        fprintf(stderr,
                "Error: '.string' — expected '\"' but got '%c'\n", line[i]);
        return;
    }
    i++; /* skip opening quote */

    /* Store characters until closing quote or end of string */
    while (line[i] != '"' && line[i] != '\0' && line[i] != '\n')
    {
        dataImage[*DC] = (unsigned char)line[i];
        (*DC)++;
        i++;
    }

    if (line[i] != '"')
    {
        fprintf(stderr, "Error: '.string' — missing closing '\"'\n");
        /* Still store the null terminator */
    }

    /* Null terminator */
    dataImage[*DC] = 0;
    (*DC)++;
}
