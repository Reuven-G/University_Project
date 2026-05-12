#include "firstPass.h"



/*
that script manages the instruction counter
*/




static int IC = IC_START;




/* add number to IC */
void updateIC(int delta)
{
    IC += delta;
}




/* return value of IC */
int getIC(void)
{
    return IC;
}




/* reset IC to 100 */
void resetIC(void)
{
    IC = IC_START;
}
