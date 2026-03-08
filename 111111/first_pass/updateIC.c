#include <stdio.h>

static int IC = 100;

void updateIC(void)
{
    IC++;

    printf("IC updated: %d\n", IC);
}
