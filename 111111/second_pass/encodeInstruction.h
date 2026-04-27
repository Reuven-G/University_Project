#ifndef ENCODE_INSTRUCTION_H
#define ENCODE_INSTRUCTION_H

#include "../instructions/instructionTable.h"

/* ARE bit values (sit in bit 0 of every machine word) */
#define ARE_ABSOLUTE   0  /* A — built-in constant, immediate value    */
#define ARE_RELOCATABLE 1 /* R — defined in this file, needs relocation */
#define ARE_EXTERNAL   2  /* E — defined in another file               */

/* Build the 12-bit base word for an instruction.
   Layout:
     bits 11-8  opcode  (4 bits)
     bits  7-5  funct   (3 bits)
     bits  4-3  src addressing mode (2 bits)
     bits  2-1  dst addressing mode (2 bits)
     bit   0    ARE = 0 (Absolute — the base word is always absolute)

   Pass addrSrc = -1 and addrDst = -1 for zero-operand instructions.
   Pass addrSrc = -1 for single-operand instructions (dst only). */
int encodeBaseWord(Instruction *inst, int addrSrc, int addrDst);

#endif
