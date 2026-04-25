#include "instructionTable.h"

int checkOperands(Instruction *inst, int srcType, int dstType)
{
    if (inst->operands == 2)
    {
        if (!inst->srcAllowed[srcType])
            return 0;

        if (!inst->dstAllowed[dstType])
            return 0;
    }

    if (inst->operands == 1)
    {
        if (!inst->dstAllowed[dstType])
            return 0;
    }

    return 1;
}
