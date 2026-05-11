#ifndef ENCODE_OPERAND_H
#define ENCODE_OPERAND_H

#include "../files/files.h"





/* encode the operand after chcking it type */
int encodeOperand(const char *operand, int addrType, int currentIC, int isSrc, int *codeImage, char *areImage, int imageIndex, ExtRef *extRefs, int *extCount);




/* pack two registers into one word */
int encodeRegisterPair(const char *srcOp, const char *dstOp);

#endif

