#include "encodeInstruction.h"
#include "../addressing/addressingType.h"

int encodeBaseWord(Instruction *inst, int addrSrc, int addrDst)
{
    int word = 0;

    /* bits 11-8: opcode (4 bits) */
    word |= (inst->opcode & 0xF) << 8;

    /* bits 7-4: funct (4 bits) — NOT 3 bits */
    word |= (inst->funct & 0xF) << 4;

    /* bits 3-2: source addressing mode (2 bits) */
    if (addrSrc >= 0)
        word |= (addrSrc & 0x3) << 2;

    /* bits 1-0: destination addressing mode (2 bits) */
    if (addrDst >= 0)
        word |= (addrDst & 0x3);

    return word & 0xFFF;
}
