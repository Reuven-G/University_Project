#include "firstPass.h"

static int IC = IC_START;

void updateIC(int delta)
{
    IC += delta;
}

int getIC(void)
{
    return IC;
}

void resetIC(void)
{
    IC = IC_START;
}
