#include "instructionTable.h"




/* validate the source and destination addressing modes */
int checkOperands(Instruction *inst, int srcType, int dstType)
{
	/* commands that need two operands */
    if (inst->operands == 2)
    {
        if (!inst->srcAllowed[srcType])
            return 0;

        if (!inst->dstAllowed[dstType])
            return 0;
    }

	/* commands that need one operands */
    if (inst->operands == 1)
    {
        if (!inst->dstAllowed[dstType])
            return 0;
    }

    return 1;
}
