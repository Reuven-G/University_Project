#include <stdlib.h>
#include <string.h>
#include "directives.h"

void handleData(char *line, int *DC, int dataImage[])
{
    char *num;

    num = strtok(line, ",");

    while(num)
    {
        dataImage[*DC] = atoi(num);
        (*DC)++;

        num = strtok(NULL,",");
    }
}
