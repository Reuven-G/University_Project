#include "calcInstructionSize.h"
#include "../addressing/addressingType.h"

/* The function calculates how many words in memory the instruction will occupy */
int calcInstructionSize(int numOperands, int addrSrc, int addrDst)
{
    int L = 1; /* Base word is always present */

    if (numOperands == 0)
    {
        return L;
    }

    if (numOperands == 1)
    {
        /* A single operand always takes exactly one extra word */
        L += 1;
        return L;
    }

    /* For two operands: */
    if (addrSrc == REGISTER && addrDst == REGISTER)
    {
        /* Special case: both registers share a single extra word */
        L += 1;
    }
    else
    {
        /* Each operand gets its own extra word */
        L += 2;
    }

    return L;
}
