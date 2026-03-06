#include <stdio.h>
#include <string.h>

#include "firstPass.h"
#include "../analyzeRow/analyzeRow.h"

/* counters */
static int IC = 100;  /* instruction counter */
static int DC = 0;    /* data Counter */

/* function to read and analyze every line */
void runFirstPass(FILE *fp)
{
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        analyzeRow(line);

        IC++;
    }

    printf("\nFirst pass finished.\n");
    printf("IC = %d\n", IC);
    printf("DC = %d\n", DC);
}
