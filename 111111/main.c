#include <stdio.h>
#include <stdlib.h>

#include "first_pass/firstPass.h"


int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 2)
    {
        printf("Error: missing input file name\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error: cannot open file %s\n", argv[1]);
        return 1;
    }

    runFirstPass(fp);

    fclose(fp);

    return 0;
}
