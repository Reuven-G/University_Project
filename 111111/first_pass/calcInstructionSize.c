#include "calcInstructionSize.h"
#include "../addressing/addressingType.h"

/* Per the 12-bit architecture spec:
   - Every instruction takes 1 base word.
   - Each operand adds 1 extra word, EXCEPT:
     if BOTH src and dst are REGISTER mode, they share a single
     extra word (register pair encoding), so only +1 total instead of +2. */

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

    /* numOperands == 2 */
    if (addrSrc == REGISTER && addrDst == REGISTER)
    {
        /* both registers share one extra word */
        L += 1;
    }
    else
    {
        L += 1; /* src word */
        L += 1; /* dst word */
    }

    return L;
}
