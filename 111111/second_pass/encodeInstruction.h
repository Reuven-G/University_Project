#ifndef ENCODE_INSTRUCTION_H
#define ENCODE_INSTRUCTION_H

#include "../instructions/instructionTable.h"

/* define the A.R.E flags */
#define ARE_ABSOLUTE   0  /* A */
#define ARE_RELOCATABLE 1 /* R */
#define ARE_EXTERNAL   2  /* E */




/* it builds 12-bit instruction word */
int encodeBaseWord(Instruction *inst, int addrSrc, int addrDst);

#endif
