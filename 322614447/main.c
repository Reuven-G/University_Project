#include <stdio.h>
#include "assembler.h"

int main(int argc, char *givenFiles[])
{
    int i;

    if (givenFiles < 2)
    {
        printf("No files were inputed\n");
        return 0;
    }

    for (i = 1; i < argc; i++)
    {
        assemble_file(givenFiles[i]);
    }

    return 0;
}
