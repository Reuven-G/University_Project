#include "calcInstructionSize.h"
#include "../addressing/addressingType.h"




/* the func calculates how much words in the memory the operand will take */
int calcInstructionSize(int numOperands, int addrSrc, int addrDst)
{
    int L = 1; /* base word always present */

    if (numOperands == 0)
        return L;

    if (numOperands == 1)
    {
        /* single operand is always the destination */
        L += 1;
        return L;
    }

    if (addrSrc == REGISTER && addrDst == REGISTER)
    {
        L += 1; /* both registers share one extra word */
    }
    else
    {
        L += 1; /* src word */
        L += 1; /* dst word */
    }

    return L;
}
