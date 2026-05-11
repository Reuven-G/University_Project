#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encodeOperand.h"
#include "encodeInstruction.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"

/* Register N is encoded as bitmask: 1 << N */
static int regMask(const char *op)
{
    return 1 << (op[1] - '0');
}

/* ── IMMEDIATE: value stored directly in 12-bit word, ARE=A ── */
static int encodeImmediate(const char *operand)
{
    int value = atoi(operand + 1); /* skip '#' */
    return value & 0xFFF;
}

/* ── DIRECT: symbol address stored directly, ARE=R (internal) or E (extern) ── */
static int encodeDirect(const char *operand, int currentIC,
                         ExtRef *extRefs, int *extCount, char *are)
{
    Symbol *sym = findSymbol((char *)operand);

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand);
        *are = 'A';
        return 0;
    }

    if (sym->type == EXTERN_LABEL)
    {
        if (*extCount < 256)
        {
            strncpy(extRefs[*extCount].name, operand, 30);
            extRefs[*extCount].name[30] = '\0';
            extRefs[*extCount].address  = currentIC;
            (*extCount)++;
        }
        *are = 'E';
        return 0; /* address = 0 for external */
    }

    *are = 'R';
    return sym->address & 0xFFF;
}

/* ── RELATIVE: (target - currentIC) stored directly, ARE=R ── */
static int encodeRelative(const char *operand, int currentIC)
{
    Symbol *sym;
    int     distance;

    sym = findSymbol((char *)(operand + 1)); /* skip '%' */

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand + 1);
        return 0;
    }

    distance = sym->address - currentIC;
    return distance & 0xFFF;
}

/* ── REGISTER (single operand): bitmask of register number, ARE=A ── */
static int encodeRegister(const char *operand)
{
    return regMask(operand) & 0xFFF;
}

/* ── Register pair: both in one word, ARE=A ── */
int encodeRegisterPair(const char *srcOp, const char *dstOp)
{
    return (regMask(srcOp) | regMask(dstOp)) & 0xFFF;
}

/* ── Public: encode one operand ── */
int encodeOperand(const char *operand, int addrType,
                  int currentIC, int isSrc,
                  int *codeImage, char *areImage, int imageIndex,
                  ExtRef *extRefs, int *extCount)
{
    int  word = 0;
    char are  = 'A';

    switch (addrType)
    {
        case IMMEDIATE:
            word = encodeImmediate(operand);
            are  = 'A';
            break;

        case DIRECT:
            word = encodeDirect(operand, currentIC,
                                extRefs, extCount, &are);
            break;

        case RELATIVE:
            word = encodeRelative(operand, currentIC);
            are  = 'R';
            break;

        case REGISTER:
            word = encodeRegister(operand);
            are  = 'A';
            (void)isSrc; /* register bitmask is same for src and dst */
            break;

        default:
            fprintf(stderr, "Error: unknown addressing type %d\n", addrType);
            return 0;
    }

    codeImage[imageIndex] = word;
    areImage[imageIndex]  = are;
    return 1;
}

