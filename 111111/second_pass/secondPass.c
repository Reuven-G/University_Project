#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "secondPass.h"
#include "encodeInstruction.h"
#include "encodeOperand.h"
#include "../first_pass/firstPass.h"
#include "../first_pass/parseOperands.h"
#include "../utils/utils.h"
#include "../labels/labels.h"
#include "../directives/directives.h"
#include "../instructions/instructionTable.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"
#include "../files/files.h"

#define MAX_LINE_LEN   82
#define MAX_TOKEN_LEN  82
#define MAX_CODE_IMAGE 4096
#define MAX_EXT_REFS   512

/* ── Internal state ─────────────────────────────────────────────── */

static int  codeImage[MAX_CODE_IMAGE];
static char areImage[MAX_CODE_IMAGE];  /* parallel ARE per word */
static int  codeIndex = 0;

static ExtRef extRefs[MAX_EXT_REFS];
static int    extCount = 0;

/* ── Helpers ────────────────────────────────────────────────────── */

/* Read the next whitespace-delimited token from line[i] into dest.
   Returns the new index after the token. */
static int readToken(const char *line, int i, char *dest)
{
    int j = 0;
    i = skipWhiteChars(line, i);
    while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        dest[j++] = line[i++];
    dest[j] = '\0';
    return i;
}

/* Skip a "LABEL:" prefix if present.
   Returns index past the colon, or 0 if no label found. */
static int skipLabelIndex(const char *line)
{
    int i = 0;

    /* skip leading whitespace */
    i = skipWhiteChars(line, i);

    /* read first word */
    while (line[i] && line[i] != ':' && line[i] != ' ' &&
           line[i] != '\t' && line[i] != '\n')
        i++;

    if (line[i] == ':')
        return i + 1; /* skip past the colon */

    return 0; /* no label — start from beginning */
}

/* ── Write one word to codeImage with its ARE letter ── */
static void writeWord(int word, char are)
{
    codeImage[codeIndex] = word & 0xFFF;
    areImage[codeIndex]  = are;
    codeIndex++;
}

/* ── Instruction encoder ────────────────────────────────────────── */

static int encodeInstructionLine(const char *line, int lineNum)
{
    char  command[MAX_TOKEN_LEN];
    char  operandBuf[MAX_TOKEN_LEN];
    char  op1[MAX_TOKEN_LEN];
    char  op2[MAX_TOKEN_LEN];
    int   i = 0;
    int   numOps;
    int   addrSrc    = -1;
    int   addrDst    = -1;
    int   baseWord;
    int   baseIndex;   /* index of the base word in codeImage */
    int   currentAddr;
    Instruction *inst;

    i = readToken(line, 0, command);
    if (command[0] == '\0') return 1;

    inst = findInstruction(command);
    if (inst == NULL) return 1;

    i = skipWhiteChars(line, i);
    strncpy(operandBuf, line + i, MAX_TOKEN_LEN - 1);
    operandBuf[MAX_TOKEN_LEN - 1] = '\0';

    numOps = parseOperands(operandBuf, op1, op2);

    if (numOps == 2)
    {
        addrSrc = getAddressingType(op1);
        addrDst = getAddressingType(op2);
    }
    else if (numOps == 1)
    {
        addrDst = getAddressingType(op1);
    }

    /* Base word — record its index, write it, mark ARE=A */
    baseIndex   = codeIndex;
    baseWord    = encodeBaseWord(inst, addrSrc, addrDst);
    currentAddr = IC_START + codeIndex;
    codeImage[codeIndex] = baseWord;
    areImage[codeIndex]  = 'A';       /* base word is ALWAYS Absolute */
    codeIndex++;
    currentAddr++;

    if (numOps == 0)
        return 1;

    if (numOps == 2)
    {
        if (addrSrc == REGISTER && addrDst == REGISTER)
        {
            codeImage[codeIndex] = encodeRegisterPair(op1, op2);
            areImage[codeIndex]  = 'A';
            codeIndex++;
            return 1;
        }

        /* src extra word */
        encodeOperand(op1, addrSrc, currentAddr, 1,
                      codeImage, areImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
        currentAddr++;

        /* dst extra word */
        encodeOperand(op2, addrDst, currentAddr, 0,
                      codeImage, areImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
    }
    else /* numOps == 1 */
    {
        encodeOperand(op1, addrDst, currentAddr, 0,
                      codeImage, areImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
    }

    (void)baseIndex; /* suppress unused warning */
    return 1;
}

/* ── Main driver ────────────────────────────────────────────────── */

void runSecondPass(const char *baseName)
{
    char  amName[256];
    FILE *fp;
    char  line[MAX_LINE_LEN];
    char  command[MAX_TOKEN_LEN];
    int   lineNum  = 0;
    int   hasError = 0;
    int   labelEnd;
    int  *dataImage;
    int   dcSize;
    int   icSize;
    Symbol *sym;

    /* Build .am filename */
    strncpy(amName, baseName, 251);
    amName[251] = '\0';
    strcat(amName, ".am");

    fp = fopen(amName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot open '%s' for second pass\n", amName);
        return;
    }

    /* Reset state */
    codeIndex = 0;
    extCount  = 0;
    memset(areImage, 'A', sizeof(areImage));

    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;

        if (isEmptyLine(line) || isComment(line))
            continue;

        /* Find where the actual command starts (skip optional label) */
        labelEnd = skipLabelIndex(line);

        /* Peek at the command name to filter directives */
        readToken(line, labelEnd, command);

        if (strcmp(command, ".entry")  == 0 ||
            strcmp(command, ".extern") == 0 ||
            strcmp(command, ".data")   == 0 ||
            strcmp(command, ".string") == 0)
            continue;

        /* Encode as instruction */
        if (!encodeInstructionLine(line + labelEnd, lineNum))
            hasError = 1;
    }

    fclose(fp);

    if (hasError)
    {
        fprintf(stderr, "Second pass finished with errors.\n");
        return;
    }

    /* Validate all .entry symbols have a real address */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL && sym->address == 0)
        {
            fprintf(stderr,
                    "Error: .entry '%s' was never defined in this file\n",
                    sym->name);
            hasError = 1;
        }
        sym = sym->next;
    }
    if (hasError) return;

    /* Write output files */
    icSize    = codeIndex;
    dataImage = getDataImage();
    dcSize    = getDC();

    writeObFile(baseName, codeImage, areImage, icSize, dataImage, dcSize);
    writeEntFile(baseName);
    writeExtFile(baseName, extRefs, extCount);

    printf("Done: %s  (IC=%d, DC=%d)\n", baseName, icSize, dcSize);
}

