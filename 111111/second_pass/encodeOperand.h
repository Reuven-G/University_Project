#ifndef ENCODE_OPERAND_H
#define ENCODE_OPERAND_H

#include "../files/files.h"

/* Encode one operand and write its extra word(s) into codeImage[].
   Returns the number of words written (0, 1, or shared-register case).

   Parameters:
     operand    — the operand string as it appears in source (e.g. "#5", "LOOP", "%LABEL", "r3")
     addrType   — addressing mode (IMMEDIATE / DIRECT / RELATIVE / REGISTER)
     currentIC  — address of this extra word in memory (used for relative calc and ext log)
     isSrc      — 1 if this is the source operand, 0 if destination
                  (used only for register-pair encoding)
     codeImage  — array to write the word into
     imageIndex — index into codeImage where the word should go
     extRefs    — array to record external references into
     extCount   — pointer to current count of external references
*/
int encodeOperand(const char *operand, int addrType,
                  int currentIC, int isSrc,
                  int *codeImage, int imageIndex,
                  ExtRef *extRefs, int *extCount);

/* Encode a register-pair extra word when BOTH src and dst are registers.
   src register index in bits 5-3, dst register index in bits 2-0.
   ARE = 0 (Absolute). Returns the encoded word. */
int encodeRegisterPair(const char *srcOp, const char *dstOp);

#endif
