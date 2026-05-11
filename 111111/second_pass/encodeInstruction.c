#include "encodeInstruction.h"
#include "../addressing/addressingType.h"




/* it builds 12-bit instruction word */
int encodeBaseWord(Instruction *inst, int addrSrc, int addrDst)
{
    int word = 0;

    /* bits 11-8: opcode */
    word |= (inst->opcode & 0xF) << 8;

    /* bits 7-4: funct */
    word |= (inst->funct & 0xF) << 4;

    /* bits 3-2: source addressing mode */
    if (addrSrc >= 0)
        word |= (addrSrc & 0x3) << 2;

    /* bits 1-0: destination addressing mode */
    if (addrDst >= 0)
        word |= (addrDst & 0x3);

    return word & 0xFFF;
}
