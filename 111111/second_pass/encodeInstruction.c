#include "encodeInstruction.h"
#include "../addressing/addressingType.h"

int encodeBaseWord(Instruction *inst, int addrSrc, int addrDst)
{
    int word = 0;

    /* bits 11-8: opcode (4 bits) */
    word |= (inst->opcode & 0xF) << 8;

    /* bits 7-5: funct (3 bits) */
    word |= (inst->funct & 0x7) << 5;

    /* bits 4-3: source addressing mode (2 bits), 0 if unused */
    if (addrSrc >= 0)
        word |= (addrSrc & 0x3) << 3;

    /* bits 2-1: destination addressing mode (2 bits), 0 if unused */
    if (addrDst >= 0)
        word |= (addrDst & 0x3) << 1;

    /* bit 0: ARE = 0 (Absolute) — base word is always absolute */
    word |= ARE_ABSOLUTE;

    return word & 0xFFF; /* mask to 12 bits */
}
