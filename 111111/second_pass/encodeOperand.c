#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encodeOperand.h"
#include "encodeInstruction.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"




/* turns register's number to bitmask */
static int regMask(const char *op)
{
    return 1 << (op[1] - '0');
}




/* turns immediate to binary number (12-bit) */
static int encodeImmediate(const char *operand)
{
    int value = atoi(operand + 1); /* +1 to skip # */
    return value & 0xFFF; /* keep only 12 bits */
}




/* searches for the address in the symbol table */
static int encodeDirect(const char *operand, int currentIC, ExtRef *extRefs, int *extCount, char *are)
{
    Symbol *sym = findSymbol((char *)operand);

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand);
        *are = 'A';
        return 0;
    }

	/* if its external label log it to extRefs */
    if (sym->type == EXTERN_LABEL)
    {
        if (*extCount < 256)
        {
            strncpy(extRefs[*extCount].name, operand, 30);
            extRefs[*extCount].name[30] = '\0';
            extRefs[*extCount].address  = currentIC;
            (*extCount)++;
        }
        *are = 'E'; /* external label marked as "E" */
        return 0;
    }

    *are = 'R'; /* local label marked as "R" */
    return sym->address & 0xFFF;
}




/* calculate distance between current IC and target label */
static int encodeRelative(const char *operand, int currentIC)
{
    Symbol *sym;
    int     distance;

    sym = findSymbol((char *)(operand + 1));

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand + 1);
        return 0;
    }

    distance = sym->address - currentIC; /* distance = place_to_go - place_now */
    return distance & 0xFFF;
}




/* returns the register as a bitmask */
static int encodeRegister(const char *operand)
{
    return regMask(operand) & 0xFFF;
}




/* pack two registers into one word */
int encodeRegisterPair(const char *srcOp, const char *dstOp)
{
    return (regMask(srcOp) | regMask(dstOp)) & 0xFFF;
}




/* encode the operand after chcking it type */
int encodeOperand(const char *operand, int addrType, int currentIC, int isSrc, int *codeImage, char *areImage, int imageIndex, ExtRef *extRefs, int *extCount)
{
    int  word = 0;
    char are  = 'A'; /* default */

    switch (addrType)
    {
        case IMMEDIATE:
            word = encodeImmediate(operand);
            are  = 'A';
            break;

        case DIRECT:
            word = encodeDirect(operand, currentIC, extRefs, extCount, &are);
            break;

        case RELATIVE:
            word = encodeRelative(operand, currentIC);
            are  = 'R';
            break;

        case REGISTER:
            word = encodeRegister(operand);
            are  = 'A';
            (void)isSrc;
            break;

        default:
            fprintf(stderr, "Error: unknown addressing type %d\n", addrType);
            return 0;
    }

    codeImage[imageIndex] = word;
    areImage[imageIndex]  = are;
    return 1;
}

