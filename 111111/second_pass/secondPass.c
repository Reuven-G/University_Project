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


static int  codeImage[MAX_CODE_IMAGE];
static char areImage[MAX_CODE_IMAGE]; 
static int  codeIndex = 0;

static ExtRef extRefs[MAX_EXT_REFS];
static int    extCount = 0;




/*  read next word in the line */
static int readToken(const char *line, int i, char *dest)
{
    int j = 0;
    i = skipWhiteChars(line, i);
    while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        dest[j++] = line[i++];
    dest[j] = '\0';
    return i;
}




/* skips label */
static int skipLabelIndex(const char *line)
{
    int i = 0;

    /* skip whitespace */
    i = skipWhiteChars(line, i);

    /* read first word */
    while (line[i] && line[i] != ':' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        i++;

    if (line[i] == ':')
        return i + 1; /* return place after the label */

    return 0; /* label not found, start from start */
}




/* write word */
static void writeWord(int word, char are)
{
    codeImage[codeIndex] = word & 0xFFF;
    areImage[codeIndex]  = are;
    codeIndex++;
}




/* the encoder of the line of the command */
static int encodeInstructionLine(const char *line, int lineNum)
{
    char         command[MAX_TOKEN_LEN];
    char         operandBuf[MAX_TOKEN_LEN];
    char         op1[MAX_TOKEN_LEN];
    char         op2[MAX_TOKEN_LEN];
    int          i = 0;
    int          numOps;
    int          addrSrc    = -1;
    int          addrDst    = -1;
    int          baseWord;
    int          baseIndex;
    int          currentAddr;
    Instruction *inst;
 
	/* check what command we got */
    i = readToken(line, 0, command);
    if (command[0] == '\0') return 1;
    inst = findInstruction(command);
    if (inst == NULL) return 1;
 
	/* check what is the operands */
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
 
	/* encode the first word */
    baseIndex            = codeIndex;
    baseWord             = encodeBaseWord(inst, addrSrc, addrDst);
    currentAddr          = IC_START + codeIndex;
    codeImage[codeIndex] = baseWord;
    areImage[codeIndex]  = 'A';
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
 
		/* source and destination */
        encodeOperand(op1, addrSrc, currentAddr, 1, codeImage, areImage, codeIndex, extRefs, &extCount);
        codeIndex++;
        currentAddr++;
        encodeOperand(op2, addrDst, currentAddr, 0, codeImage, areImage, codeIndex, extRefs, &extCount);
        codeIndex++;
    }
    else /* only one operand */
    {
        encodeOperand(op1, addrDst, currentAddr, 0, codeImage, areImage, codeIndex, extRefs, &extCount);
        codeIndex++;
    }
 
    (void)baseIndex;
    return 1;
}



/* second pass on the file */
void runSecondPass(const char *baseName)
{
    char    amName[256];
    FILE   *fp;
    char    line[MAX_LINE_LEN];
    char    command[MAX_TOKEN_LEN];
    int     lineNum  = 0;
    int     hasError = 0;
    int     labelEnd;
    int    *dataImage;
    int     dcSize;
    int     icSize;
    Symbol *sym;
 
    strncpy(amName, baseName, 251);
    amName[251] = '\0';
    strcat(amName, ".am");
 
    fp = fopen(amName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot open '%s' for second pass\n", amName);
        return;
    }
 
	/* reset before start */
    codeIndex = 0;
    extCount  = 0;
 
	/* loop on the file and encode everything */
    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;
 
		/* ignore empty lines and comments */
        if (isEmptyLine(line) || isComment(line))
            continue;
 
        labelEnd = skipLabelIndex(line);
        readToken(line, labelEnd, command);
 
		/* it is already handled in pass 1 */
        if (strcmp(command, ".entry")  == 0 || strcmp(command, ".extern") == 0 || strcmp(command, ".data")   == 0 || strcmp(command, ".string") == 0)
            continue;
 
		/* trying to encode the instruction */
        if (!encodeInstructionLine(line + labelEnd, lineNum))
            hasError = 1;
    }
 
    fclose(fp);
 
    if (hasError)
    {
        fprintf(stderr, "Second pass finished with errors.\n");
        return;
    }
 
	/* check if we have .entry for label hat dont exist */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL && sym->address == 0)
        {
            fprintf(stderr, "Error: .entry '%s' was never defined in this file\n", sym->name);
            hasError = 1;
        }
        sym = sym->next;
    }
    if (hasError) return;
 
 
	/* all good */
    icSize    = codeIndex;
    dataImage = getDataImage();
    dcSize    = getDC();
 
    writeObFile(baseName, codeImage, areImage, icSize, dataImage, dcSize);
    writeEntFile(baseName);
    writeExtFile(baseName, extRefs, extCount);
 
    printf("Done: %s  (IC=%d, DC=%d)\n", baseName, icSize, dcSize);
}
