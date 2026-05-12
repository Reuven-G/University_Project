#include "firstPass.h"




/* the data counter script */




static int DC = 0;
static int dataImage[MAX_DATA_IMAGE];




/* add to the DC */
void updateDC(int delta)
{
    DC += delta;
}




/* return the value of DC */
int getDC(void)
{
    return DC;
}



 /* reset DC to 0 */
void resetDC(void)
{
    DC = 0;
}



/* gives access to the data array */
int *getDataImage(void)
{
    return dataImage;
}
