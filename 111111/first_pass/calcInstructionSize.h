#ifndef CALC_INSTRUCTION_SIZE_H
#define CALC_INSTRUCTION_SIZE_H

/* Returns L: the number of memory words an instruction occupies.
   L = 1 (the base word) + 1 extra word per operand, EXCEPT when
   both src and dst are registers — they share one extra word.

   addrSrc / addrDst: addressing mode constants (IMMEDIATE=0, DIRECT=1,
                      RELATIVE=2, REGISTER=3). Pass -1 if not used. */
int calcInstructionSize(int numOperands, int addrSrc, int addrDst);

#endif
