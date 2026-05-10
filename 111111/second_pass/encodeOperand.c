#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encodeOperand.h"
#include "encodeInstruction.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"

/* Register N is encoded as 1<<N (bitmask, not the number itself) */
static int regMask(const char *op)
{
    return 1 << (op[1] - '0');
}

static int encodeImmediate(const char *operand)
{
    int value = atoi(operand + 1); /* skip '#' */
    return value & 0xFFF;          /* store directly, no shift */
}

static int encodeDirect(const char *operand,
                         int currentIC,
                         ExtRef *extRefs, int *extCount)
{
    Symbol *sym = findSymbol((char *)operand);

    if (sym == NULL)
    {
        fprintf(stderr, "Error: undefined symbol '%s'\n", operand);
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
        return 0; /* address=0 for extern, ARE=E shown separately */
    }

    /* Internal: store address directly in 12-bit word */
    return sym->address & 0xFFF;
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
    int word = regMask(operand);
    return word & 0xFFF;
    /* Note: src and dst single-register words are the same —
       just the bitmask of the register. ARE=0 (Absolute). */
}

/* ── Public: register pair ────────────────────────────────────────
   Both src and dst are registers — they share one word.
   src in bits 5-3, dst in bits 2-0, ARE = 0. */
int encodeRegisterPair(const char *srcOp, const char *dstOp)
{
    /* Both registers in one word: src_mask | dst_mask */
    int word = regMask(srcOp) | regMask(dstOp);
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
