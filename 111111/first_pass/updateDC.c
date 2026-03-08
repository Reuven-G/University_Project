#include <stdio.h>

static int DC = 0;

void updateDC(void)
{
    DC++;

    printf("DC updated: %d\n", DC);
}
