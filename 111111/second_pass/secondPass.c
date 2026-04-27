#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "secondPass.h"
#include "../first_pass/firstPass.h"
#include "../first_pass/parseOperands.h"
#include "../first_pass/calcInstructionSize.h"
#include "../utils/utils.h"
#include "../labels/labels.h"
#include "../directives/directives.h"
#include "../instructions/instructionTable.h"
#include "../addressing/addressingType.h"
#include "../symbolTable/symbolTable.h"
#include "../files/files.h"
#include "encodeInstruction.h"
#include "encodeOperand.h"

#define MAX_LINE_LEN   82
#define MAX_TOKEN_LEN  82
#define MAX_LABEL_LEN  31
#define MAX_CODE_IMAGE 4096
#define MAX_EXT_REFS   512

/* ── Internal state ─────────────────────────────────────────────── */

static int    codeImage[MAX_CODE_IMAGE];
static int    codeIndex = 0;   /* next free slot in codeImage */

static ExtRef extRefs[MAX_EXT_REFS];
static int    extCount = 0;

/* ── Helpers ────────────────────────────────────────────────────── */

/* Skip past "label:" prefix if present, return pointer to command */
static char *skipLabel(char *line)
{
    char *colon = strchr(line, ':');
    if (colon != NULL)
        return colon + 1;
    return line;
}

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

/* ── Instruction encoder ────────────────────────────────────────── */

static int encodeInstructionLine(char *line, int lineNum)
{
    char  command[MAX_TOKEN_LEN];
    char  operandBuf[MAX_TOKEN_LEN];
    char  op1[MAX_TOKEN_LEN];
    char  op2[MAX_TOKEN_LEN];
    int   i = 0;
    int   numOps;
    int   addrSrc = -1;
    int   addrDst = -1;
    int   baseWord;
    int   currentAddr;
    Instruction *inst;

    /* Read command name */
    i = readToken(line, 0, command);
    if (command[0] == '\0') return 1;

    inst = findInstruction(command);
    if (inst == NULL) return 1; /* not an instruction (directive) — skip */

    /* Collect remainder as operand string */
    i = skipWhiteChars(line, i);
    strncpy(operandBuf, line + i, MAX_TOKEN_LEN - 1);
    operandBuf[MAX_TOKEN_LEN - 1] = '\0';

    /* Parse operands */
    numOps = parseOperands(operandBuf, op1, op2);

    /* Determine addressing modes */
    if (numOps >= 1)
        addrDst = getAddressingType((numOps == 1) ? op1 : op2);
    if (numOps == 2)
        addrSrc = getAddressingType(op1);

    /* ── Write base word ── */
    baseWord = encodeBaseWord(inst, addrSrc, addrDst);
    currentAddr = IC_START + codeIndex;
    codeImage[codeIndex++] = baseWord;
    currentAddr++;

    /* ── Write extra words ── */
    if (numOps == 0)
        return 1;

    if (numOps == 2)
    {
        /* Both registers share one word */
        if (addrSrc == REGISTER && addrDst == REGISTER)
        {
            codeImage[codeIndex++] = encodeRegisterPair(op1, op2);
            return 1;
        }

        /* Source extra word */
        encodeOperand(op1, addrSrc, currentAddr, 1,
                      codeImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
        currentAddr++;

        /* Destination extra word */
        encodeOperand(op2, addrDst, currentAddr, 0,
                      codeImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
    }
    else /* numOps == 1 */
    {
        encodeOperand(op1, addrDst, currentAddr, 0,
                      codeImage, codeIndex,
                      extRefs, &extCount);
        codeIndex++;
    }

    return 1;
}

/* ── Main driver ────────────────────────────────────────────────── */

void runSecondPass(const char *baseName)
{
    char  amName[256];
    FILE *fp;
    char  line[MAX_LINE_LEN];
    char  afterLabel[MAX_LINE_LEN];
    char  command[MAX_TOKEN_LEN];
    int   lineNum = 0;
    int   hasError = 0;
    int   i;
    int  *dataImage;
    int   dcSize;
    int   icSize;

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

    /* Reset code image */
    codeIndex = 0;
    extCount  = 0;

    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;

        if (isEmptyLine(line) || isComment(line))
            continue;

        /* Strip optional label prefix */
        strncpy(afterLabel, skipLabel(line), MAX_LINE_LEN - 1);
        afterLabel[MAX_LINE_LEN - 1] = '\0';

        /* Peek at the command/directive name */
        i = readToken(afterLabel, 0, command);

        /* Skip .entry and .extern — handled in first pass */
        if (strcmp(command, ".entry")  == 0 ||
            strcmp(command, ".extern") == 0)
            continue;

        /* Skip .data and .string — data image already built */
        if (strcmp(command, ".data")   == 0 ||
            strcmp(command, ".string") == 0)
            continue;

        /* Must be an instruction */
        if (!encodeInstructionLine(afterLabel, lineNum))
            hasError = 1;
    }

    fclose(fp);

    if (hasError)
    {
        fprintf(stderr, "Second pass finished with errors — output not written.\n");
        return;
    }

    /* ── Write output files ── */
    icSize    = codeIndex;
    dataImage = getDataImage();
    dcSize    = getDC();

    writeObFile(baseName, codeImage, icSize, dataImage, dcSize);
    writeEntFile(baseName);
    writeExtFile(baseName, extRefs, extCount);

    printf("Done: %s  (IC=%d, DC=%d)\n", baseName, icSize, dcSize);
}
