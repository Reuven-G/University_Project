#include "firstPass.h"




/* the data counter script */




static int DC = 0;
static int dataImage[MAX_DATA_IMAGE];

void updateDC(int delta)
{
    DC += delta;
}

int getDC(void)
{
    return DC;
}

void resetDC(void)
{
    DC = 0;
}

int *getDataImage(void)
{
    return dataImage;
}
