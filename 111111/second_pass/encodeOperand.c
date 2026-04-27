#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encodeOperand.h"
#include "encodeInstruction.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"

/* Extract register number from "rN" string */
static int regNum(const char *op)
{
    return op[1] - '0';
}

/* ── IMMEDIATE ── bits 11-2: value (10 bits, 2's complement), bits 1-0: ARE=00 */
static int encodeImmediate(const char *operand)
{
    int value;
    int word;

    /* skip the '#' */
    value = atoi(operand + 1);

    /* value occupies bits 11-2 (10 bits) */
    word = (value & 0x3FF) << 2;

    /* ARE = 00 (Absolute) */
    word |= ARE_ABSOLUTE;

    return word & 0xFFF;
}

/* ── DIRECT ── bits 11-2: symbol address (10 bits), bits 1-0: ARE */
static int encodeDirect(const char *operand,
                         int currentIC,
                         ExtRef *extRefs, int *extCount)
{
    Symbol *sym;
    int     word;
    int     are;

    sym = findSymbol((char *)operand);

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand);
        return 0;
    }

    if (sym->type == EXTERN_LABEL)
    {
        /* External: address = 0, ARE = E (2) */
        word = 0;
        are  = ARE_EXTERNAL;

        /* Record this reference for the .ext file */
        if (*extCount < 256)
        {
            strncpy(extRefs[*extCount].name, operand, 30);
            extRefs[*extCount].name[30] = '\0';
            extRefs[*extCount].address  = currentIC;
            (*extCount)++;
        }
    }
    else
    {
        /* Internal: address in bits 11-2, ARE = R (1) */
        word = (sym->address & 0x3FF) << 2;
        are  = ARE_RELOCATABLE;
    }

    word |= are;
    return word & 0xFFF;
}

/* ── RELATIVE ── operand is "%LABEL"
   bits 11-2: (target_address - current_IC) as signed 10-bit, ARE = R (1) */
static int encodeRelative(const char *operand, int currentIC)
{
    Symbol *sym;
    int     distance;
    int     word;

    /* skip the '%' */
    sym = findSymbol((char *)(operand + 1));

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand + 1);
        return 0;
    }

    /* Distance = target - address of THIS extra word */
    distance = sym->address - currentIC;

    /* Pack signed distance into 10 bits */
    word  = (distance & 0x3FF) << 2;
    word |= ARE_RELOCATABLE;

    return word & 0xFFF;
}

/* ── REGISTER (single) ──
   src register: bits 5-3
   dst register: bits 2-0
   ARE = 0 (Absolute)
   isSrc=1 → place in bits 5-3, isSrc=0 → place in bits 2-0 */
static int encodeRegister(const char *operand, int isSrc)
{
    int reg  = regNum(operand);
    int word = 0;

    if (isSrc)
        word = (reg & 0x7) << 3;
    else
        word = (reg & 0x7);

    word |= ARE_ABSOLUTE;
    return word & 0xFFF;
}

/* ── Public: register pair ────────────────────────────────────────
   Both src and dst are registers — they share one word.
   src in bits 5-3, dst in bits 2-0, ARE = 0. */
int encodeRegisterPair(const char *srcOp, const char *dstOp)
{
    int word = 0;
    word |= (regNum(srcOp) & 0x7) << 3;
    word |= (regNum(dstOp) & 0x7);
    word |= ARE_ABSOLUTE;
    return word & 0xFFF;
}

/* ── Public: encode one operand extra word ───────────────────────── */
int encodeOperand(const char *operand, int addrType,
                  int currentIC, int isSrc,
                  int *codeImage, int imageIndex,
                  ExtRef *extRefs, int *extCount)
{
    int word = 0;

    switch (addrType)
    {
        case IMMEDIATE:
            word = encodeImmediate(operand);
            break;

        case DIRECT:
            word = encodeDirect(operand, currentIC, extRefs, extCount);
            break;

        case RELATIVE:
            word = encodeRelative(operand, currentIC);
            break;

        case REGISTER:
            word = encodeRegister(operand, isSrc);
            break;

        default:
            fprintf(stderr, "Error: unknown addressing type %d\n", addrType);
            return 0;
    }

    codeImage[imageIndex] = word;
    return 1;
}
