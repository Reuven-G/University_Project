#include "directives.h"

void handleString(char *line, int *DC, int dataImage[])
{
    int i = 1;

    while(line[i] != '"' && line[i] != '\0')
    {
        dataImage[*DC] = line[i];
        (*DC)++;
        i++;
    }

    dataImage[*DC] = 0;
    (*DC)++;
}
