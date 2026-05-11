#ifndef ENCODE_OPERAND_H
#define ENCODE_OPERAND_H

#include "../files/files.h"

/* Encode one operand extra word into codeImage[imageIndex].
   Also sets areImage[imageIndex] to 'A', 'R', or 'E'.
   Records external references into extRefs[].
   Returns 1 on success, 0 on error. */
int encodeOperand(const char *operand, int addrType,
                  int currentIC, int isSrc,
                  int *codeImage, char *areImage, int imageIndex,
                  ExtRef *extRefs, int *extCount);

/* Encode a register-pair word when both src and dst are registers.
   src register bitmask ORed with dst register bitmask. ARE = A. */
int encodeRegisterPair(const char *srcOp, const char *dstOp);

#endif

